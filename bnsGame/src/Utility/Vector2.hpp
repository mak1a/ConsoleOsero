#include <cstdint>

namespace bnsGame::utl {
	template <class T>
	class Vector2 {
		using value_t = T;
	private:
		value_t m_x;
		value_t m_y;
	
		void SetX(const value_t x_) {
			m_x = x_;
		}
		void SetY(const value_t y_) {
			m_y = y_;
		}

	public:
		value_t GetX() const {
			return m_x;
		}
		value_t GetY() const {
			return m_y;
		}

		__declspec(property(get = GetX, put = SetX)) value_t x;
		__declspec(property(get = GetY, put = SetY)) value_t y;

		Vector2() noexcept = default;

		constexpr Vector2(const Vector2& v_) noexcept
			: m_x(v_.x)
			, m_y(v_.y) {}

		constexpr Vector2(const value_t x_, const value_t y_) noexcept
			: m_x(x_)
			, m_y(y_) {}

		template <class U>
		constexpr Vector2(const Vector2<U>& v_)
			: m_x(v_.x)
			, m_y(v_.y) {}

		virtual ~Vector2() noexcept = default;

		[[nodiscard]] constexpr Vector2 operator+() const noexcept {
			return *this;
		}
		[[nodiscard]] constexpr Vector2 operator+(const Vector2& v_) const noexcept {
			return {x + v_.x, y + v_.y};
		}

		[[nodiscard]] constexpr Vector2 operator-() const noexcept {
			return {-x, -y};
		}
		[[nodiscard]] constexpr Vector2 operator-(const Vector2& v_) const noexcept {
			return {x - v_.x, y - v_.y};
		}

		[[nodiscard]] constexpr Vector2 operator*(const value_t s_) const noexcept {
			return {x * s_, y * s_};
		}
		[[nodiscard]] constexpr Vector2 operator*(const Vector2& v_) const noexcept {
			return {x * v_.x, y * v_.y};
		}

		[[nodiscard]] constexpr Vector2 operator/(const value_t s_) const noexcept {
			return *this * (static_cast<value_t>(1.0) / s_);
		}
		[[nodiscard]] constexpr Vector2 operator/(const Vector2& v_) const noexcept {
			return {x / v_.x, y / v_.y};
		}

		[[nodiscard]] constexpr Vector2& operator+=(const Vector2& v_) noexcept {
			x += v_.x;
			y += v_.y;
			return *this;
		}

		[[nodiscard]] constexpr Vector2& operator-=(const Vector2& v_) noexcept {
			x -= v_.x;
			y -= v_.y;
			return *this;
		}

		[[nodiscard]] constexpr Vector2& operator*=(const value_t s_) noexcept {
			x *= s_;
			y *= s_;
			return *this;
		}
		[[nodiscard]] constexpr Vector2& operator*=(const Vector2& v_) noexcept {
			x *= v_.x;
			y *= v_.y;
			return *this;
		}

		[[nodiscard]] constexpr Vector2& operator/=(const value_t s_) noexcept {
			*this *= (static_cast<value_t>(1.0) / s_);
		}
		[[nodiscard]] constexpr Vector2& operator/=(const Vector2& v_) noexcept {
			x /= v_.x;
			y /= v_.y;
			return *this;
		}

		[[nodiscard]] constexpr bool operator==(const Vector2& v_) const noexcept {
			return v_.x == x && v_.y == y;
		}

		[[nodiscard]] constexpr bool operator!=(const Vector2& v_) const noexcept {
			return v_.x != x || v_.y != y;
		}

		constexpr Vector2& Set(const value_t x_, const value_t y_) noexcept {
			x = x_;
			y = y_;
			return *this;
		}

		constexpr Vector2& Set(const Vector2& v_) noexcept {
			return *this = v_;
		}

		[[nodiscard]] constexpr Vector2 movedBy(const value_t x_, const value_t y_) const noexcept {
			return {x + x_, y + y_};
		}
		[[nodiscard]] constexpr Vector2 movedBy(const Vector2& v_) const noexcept {
			return {x + v_.x, y + v_.y};
		}

		constexpr Vector2& moveBy(const value_t x_, const value_t y_) noexcept {
			x += x_;
			y += y_;
			return *this;
		}
		constexpr Vector2& moveBy(const Vector2& v_) noexcept {
			return *this += v_;
		}
	};
}