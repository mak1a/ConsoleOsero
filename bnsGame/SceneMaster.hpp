#pragma once
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <functional>
#include <optional>
#include "iMemoryAllocator.hpp"


namespace bnsGame::utl {

    /// <summary>
    /// シーン管理
    /// </summary>
    /// <remarks>
    /// State にはシーンを区別するキーの型、Data にはシーン間で共有するデータの型を指定します。
    /// </remarks>
    template<class State, class Data>
    class SceneMaster;

    namespace detail {
        struct EmptyData {};
    }  // namespace detail

    class Uncopyable {
    protected:
        constexpr Uncopyable() = default;
        ~Uncopyable() = default;
        Uncopyable(const Uncopyable&) = delete;
        Uncopyable& operator=(const Uncopyable&) = delete;
    };

    /// <summary>
    /// シーン・インタフェース
    /// </summary>
    template<class State, class Data>
    class IScene : Uncopyable {
    public:
        using State_t = State;

        using Data_t = Data;

        struct InitData {
            State_t state;

            std::shared_ptr<Data_t> _s;

            SceneMaster<State_t, Data_t>* _m;

            InitData() = default;

            InitData(const State_t& _state, const std::shared_ptr<Data_t>& data, SceneMaster<State_t, Data_t>* manager) : state(_state), _s(data), _m(manager) {}
        };

    private:
        State_t m_state;

        std::shared_ptr<Data_t> m_data;

        SceneMaster<State_t, Data_t>* m_manager;

    public:
        explicit IScene(const InitData& init) : m_state(init.state), m_data(init._s), m_manager(init._m) {}

        virtual ~IScene() {}

        /// <summary>
        /// シーン遷移時（in）の更新
        /// </summary>
        /// <returns>
        /// なし
        /// </returns>
        virtual void UpdateChangeIn() {}

        /// <summary>
        /// 通常時の更新
        /// </summary>
        /// <returns>
        /// なし
        /// </returns>
        virtual void Update() {}

        /// <summary>
        /// シーン遷移時（out）の更新
        /// </summary>
        /// <returns>
        /// なし
        /// </returns>
        virtual void UpdateChangeOut() {}

        /// <summary>
        /// 通常時の描画
        /// </summary>
        /// <returns>
        /// なし
        /// </returns>
        virtual void Draw() const {}

    protected:
        [[nodiscard]] const State_t& getState() const {
            return m_state;
        }

        /// <summary>
        /// 共有データへの参照を取得します。
        /// </summary>
        /// <returns>
        /// 共有データへの参照
        /// </returns>
        [[nodiscard]] Data_t& getData() const {
            return *m_data;
        }

        /// <summary>
        /// シーンの変更を通知します。
        /// </summary>
        /// <param name="state">
        /// 次のシーンのキー
        /// </param>
        /// <param name="transitionTimeMillisec">
        /// フェードイン・アウトの時間（ミリ秒）
        /// </param>
        /// <param name="crossFade">
        /// クロスフェードを有効にするか
        /// </param>
        /// <returns>
        /// シーンの変更が可能でフェードイン・アウトが開始される場合 true, それ以外の場合は false
        /// </returns>
        bool ChangeScene(const State_t& state) {
            return m_manager->ChangeScene(state);
        }

        /// <summary>
        /// ゲームの終了を通知します。
        /// </summary>
        /// <remarks>
        /// この関数を呼ぶと、以降の SceneMaster::Update() / UpdateAndDraw() が false を返します。
        /// </remarks>
        /// <returns>
        /// なし
        /// </returns>
        void ExitGame() {
            return m_manager->ExitGame();
        }
    };

    /// <summary>
    /// シーン管理
    /// </summary>
    /// <remarks>
    /// State にはシーンを区別するキーの型、Data にはシーン間で共有するデータの型を指定します。
    /// </remarks>
    template<class State, class Data = detail::EmptyData>
    class SceneMaster {
    private:
        using Scene_t = std::shared_ptr<IScene<State, Data>>;

        using FactoryFunction_t = std::function<Scene_t()>;

        std::unordered_map<State, FactoryFunction_t> m_factories;

        std::shared_ptr<Data> m_data;

        Scene_t m_current;

        Scene_t m_next;

        Scene_t m_previous;

        State m_currentState;

        State m_nextState;

        std::optional<State> m_first;

        enum class TransitionState { None, Active, ChangeIn, ChangeOut } m_transitionState = TransitionState::None;

        bool m_isEnd = false;

        bool UpdateSingle() {
            if (m_transitionState == TransitionState::ChangeOut) {
                m_current = nullptr;

                m_current = m_factories[m_nextState]();

                if (IsEnd()) {
                    return false;
                }

                m_currentState = m_nextState;

                m_transitionState = TransitionState::ChangeIn;
            }

            if (m_transitionState == TransitionState::ChangeIn) {
                m_transitionState = TransitionState::Active;
            }

            switch (m_transitionState) {
            case TransitionState::ChangeIn:
                m_current->UpdateChangeIn();
                return !IsEnd();
            case TransitionState::Active:
                m_current->Update();
                return !IsEnd();
            case TransitionState::ChangeOut:
                m_current->UpdateChangeOut();
                return !IsEnd();
            default:
                return false;
            }
        }

        [[nodiscard]] bool IsEnd() const noexcept {
            return m_isEnd;
        }

    public:
        /// <summary>
        /// シーンのインタフェース
        /// </summary>
        using Scene = IScene<State, Data>;

        /// <summary>
        /// シーン管理を初期化します。
        /// </summary>
        /// <param name="option">
        /// シーン管理のオプション
        /// </param>
        SceneMaster() : m_data(std::make_shared<Data>()) {}

        /// <summary>
        /// シーン管理を初期化します。
        /// </summary>
        /// <param name="data">
        /// 共有データ
        /// </param>
        /// <param name="option">
        /// シーン管理のオプション
        /// </param>
        explicit SceneMaster(const std::shared_ptr<Data>& data) : m_data(data) {}

        ~SceneMaster() = default;

        /// <summary>
        /// シーンを追加します。
        /// </summary>
        /// <param name="state">
        /// シーンのキー
        /// </param>
        /// <returns>
        /// 追加に成功した場合 true, それ以外の場合は false
        /// </returns>
        template<class Scene>
        SceneMaster& Add(const State& state) {
            typename Scene::InitData initData(state, m_data, this);

            auto factory = [=]() { return std::make_shared<Scene>(initData); };

            auto it = m_factories.find(state);

            if (it != m_factories.end()) {
                it->second = factory;
            }
            else {
                m_factories.emplace(state, factory);

                if (!m_first) {
                    m_first = state;
                }
            }

            return *this;
        }

        /// <summary>
        /// 最初のシーンを初期化します。
        /// </summary>
        /// <param name="state">
        /// 最初のシーン
        /// </param>
        /// <returns>
        /// 初期化に成功した場合 true, それ以外の場合は false
        /// </returns>
        bool Init(const State& state) {
            if (m_current) {
                return false;
            }

            auto it = m_factories.find(state);

            if (it == m_factories.end()) {
                return false;
            }

            m_currentState = state;

            m_current = it->second();

            if (IsEnd()) {
                return false;
            }

            m_transitionState = TransitionState::ChangeIn;

            return true;
        }

        /// <summary>
        /// シーンを更新します。
        /// </summary>
        /// <returns>
        /// シーンの更新に成功した場合 true, それ以外の場合は false
        /// </returns>
        bool UpdateScene() {
            if (IsEnd()) {
                return false;
            }

            if (!m_current) {
                if (!m_first) {
                    return true;
                }
                else if (!Init(m_first.value())) {
                    return false;
                }
            }

            return UpdateSingle();
        }

        /// <summary>
        /// シーンを描画します。
        /// </summary>
        /// <returns>
        /// なし
        /// </returns>
        void DrawScene() const {
            if (!m_current) {
                return;
            }

            if (m_transitionState == TransitionState::Active) {
                m_current->Draw();
            }
        }

        /// <summary>
        /// シーンの更新と描画を行います。
        /// </summary>
        /// <returns>
        /// シーンの更新に成功した場合 true, それ以外の場合は false
        /// </returns>
        bool Update() {
            if (!UpdateScene()) {
                return false;
            }

            DrawScene();

            return true;
        }

        /// <summary>
        /// 共有データを取得します。
        /// </summary>
        /// <returns>
        /// 共有データへのポインタ
        /// </returns>
        [[nodiscard]] std::shared_ptr<Data> get() {
            return m_data;
        }

        /// <summary>
        /// 共有データを取得します。
        /// </summary>
        /// <returns>
        /// 共有データへのポインタ
        /// </returns>
        [[nodiscard]] const std::shared_ptr<const Data> get() const {
            return m_data;
        }

        /// <summary>
        /// シーンを変更します。
        /// </summary>
        /// <param name="state">
        /// 次のシーンのキー
        /// </param>
        /// <param name="transitionTimeMillisec">
        /// フェードイン・アウトの時間（ミリ秒）
        /// </param>
        /// <param name="crossFade">
        /// クロスフェードを有効にするか
        /// </param>
        /// <returns>
        /// シーンの変更が可能でフェードイン・アウトが開始される場合 true, それ以外の場合は false
        /// </returns>
        bool ChangeScene(const State& state) {
            if (m_factories.find(state) == m_factories.end()) {
                return false;
            }

            m_nextState = state;

            m_transitionState = TransitionState::ChangeOut;
            m_current->UpdateChangeOut();

            return true;
        }

        /// <summary>
        /// ゲームの終了を通知します。
        /// </summary>
        /// <remarks>
        /// この関数を呼ぶと、以降の SceneMaster::Update() / UpdateAndDraw() が false を返します。
        /// </remarks>
        /// <returns>
        /// なし
        /// </returns>
        void ExitGame() {
            m_isEnd = true;
        }
    };
}  // namespace bnsGame
