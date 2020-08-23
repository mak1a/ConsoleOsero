#pragma once
#include <memory>
#include <optional>
#include <unordered_map>

namespace bnsGame::utl {
    template<typename T>
    class State {
    private:
        T m_id;

    public:
        State() = delete;

        explicit State(T id_) : m_id(id_) {}

        virtual ~State() = default;

        [[nodiscard]] constexpr T GetID() const noexcept {
            return m_id;
        }

        virtual void Enter() {}

        virtual void Update() {}

        virtual void Draw() const {}

        virtual void Exit() {}
    };

    template<typename T>
    class StateMachine {
    private:
        std::unordered_map<T, std::shared_ptr<State<T>>> m_stateList;

        std::shared_ptr<State<T>> m_state;

    public:
        StateMachine() = default;

        virtual ~StateMachine() = default;

        std::optional<const T> GetCurrentState() const {
            if (m_state == nullptr) {
                return {};
            }

            return m_state->GetID();
        }

        void GoToState(const T nextStateID_) {
            auto it = m_stateList.find(nextStateID_);
            if (it == m_stateList.end()) {
                return;
            }

            if (m_state != nullptr) {
                m_state->Exit();
            }

            m_state = m_stateList[nextStateID_];
            m_state->Enter();
        }

        virtual void InitializeStateMachine() = 0;

        void AddState(const std::shared_ptr<State<T>>& state_) {
            if (state_ == nullptr) {
                return;
            }

            auto it = m_stateList.find(state_->GetID());
            if (it != m_stateList.end()) {
                return;
            }

            m_stateList[state_->GetID()] = state_;
        }

        void Update() {
            if (m_state == nullptr) {
                return;
            }

            m_state->Update();
        }

        void Draw() const {
            if (m_state == nullptr) {
                return;
            }

            m_state->Draw();
        }
    };
}  // namespace bnsGame::utl
