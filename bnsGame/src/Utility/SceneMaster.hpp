#pragma once
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <functional>
#include <optional>
#include "Utility/Allocator/iMemoryAllocator.hpp"


namespace bnsGame::utl {

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

    // インターフェース
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

        virtual void UpdateChangeIn() {}

        virtual void Update() {}

        virtual void UpdateChangeOut() {}

        virtual void Draw() const {}

    protected:
        [[nodiscard]] const State_t& getState() const {
            return m_state;
        }

        [[nodiscard]] Data_t& getData() const {
            return *m_data;
        }

        bool ChangeScene(const State_t& state) {
            return m_manager->ChangeScene(state);
        }

        void ExitGame() {
            return m_manager->ExitGame();
        }
    };

    // シーン遷移用クラス
    template<class State, class Data = detail::EmptyData>
    class SceneMaster {
    private:
        using Scene_t = std::shared_ptr<IScene<State, Data>>;

        using FactoryFunction_t = std::function<Scene_t()>;

        std::unordered_map<State, FactoryFunction_t> m_factories;

        std::shared_ptr<Data> m_data;

        Scene_t m_current;

        State m_currentState;

        State m_nextState;

        std::optional<State> m_first;

        enum class TransitionState { None, Active, ChangeIn, ChangeOut } m_transitionState = TransitionState::None;

        bool m_isEnd = false;

        bool UpdateState() {
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
        // 各シーンクラスで継承する
        using Scene = IScene<State, Data>;

        SceneMaster() : m_data(std::make_shared<Data>()) {}

        explicit SceneMaster(const std::shared_ptr<Data>& data) : m_data(data) {}

        ~SceneMaster() = default;

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

            return UpdateState();
        }

        void DrawScene() const {
            if (!m_current) {
                return;
            }

            if (m_transitionState == TransitionState::Active) {
                m_current->Draw();
            }
        }

        bool Update() {
            if (!UpdateScene()) {
                return false;
            }

            DrawScene();

            return true;
        }

        [[nodiscard]] std::shared_ptr<Data> get() {
            return m_data;
        }

        [[nodiscard]] const std::shared_ptr<const Data> get() const {
            return m_data;
        }

        bool ChangeScene(const State& state) {
            if (m_factories.find(state) == m_factories.end()) {
                return false;
            }

            m_nextState = state;

            m_transitionState = TransitionState::ChangeOut;
            m_current->UpdateChangeOut();

            return true;
        }

        void ExitGame() {
            m_isEnd = true;
        }
    };
}  // namespace bnsGame
