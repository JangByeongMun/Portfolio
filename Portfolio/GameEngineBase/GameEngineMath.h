#pragma once
#include <math.h>
#include <Windows.h>

// 설명 :
class GameEngineMath
{
public:
	static const float PIE;
	static const float DEG;
	static const float DegreeToRadian;

public:
	// constrcuter destructer
	GameEngineMath();
	~GameEngineMath();

	// delete Function
	GameEngineMath(const GameEngineMath& _Other) = delete;
	GameEngineMath(GameEngineMath&& _Other) noexcept = delete;
	GameEngineMath& operator=(const GameEngineMath& _Other) = delete;
	GameEngineMath& operator=(GameEngineMath&& _Other) noexcept = delete;

protected:

private:

};

class float4
{
	static float4 DegreeToDirectionFloat4(float _Degree)
	{
		return RadianToDirectionFloat4(_Degree * GameEngineMath::DegreeToRadian);
	}

	static float4 RadianToDirectionFloat4(float _Radian)
	{
		return { cosf(_Radian), sinf(_Radian) };
	}

	static float4 VectorRotationToDegreeZ(const float4& _Value, float _Degree)
	{
		return VectorRotationToRadianZ(_Value, _Degree * GameEngineMath::DegreeToRadian);
	}

	static float4 VectorRotationToRadianZ(const float4& _Value, float _Radian)
	{
		float4 Rot;
		Rot.x = _Value.x * cosf(_Radian) - _Value.y * sinf(_Radian);
		Rot.y = _Value.x * sinf(_Radian) + _Value.y * cosf(_Radian);
		return Rot;
	}

public:
	static float4 LEFT;
	static float4 RIGHT;
	static float4 UP;
	static float4 DOWN;
	static float4 ZERO;

public:
	float x;
	float y;
	float z;
	float w;

public:
	bool IsZero2D() const
	{
		return x == 0.0f && y == 0.0f;
	}

public:
	inline int ix() const
	{
		return static_cast<int>(x);
	}
	inline int iy() const
	{
		return static_cast<int>(y);
	}
	inline int iz() const
	{
		return static_cast<int>(z);
	}
	inline int iw() const
	{
		return static_cast<int>(w);
	}

	inline int hix() const
	{
		return static_cast<int>(x * 0.5f);
	}
	inline int hiy() const
	{
		return static_cast<int>(y * 0.5f);
	}
	inline int hiz() const
	{
		return static_cast<int>(z * 0.5f);
	}

	inline float4 Half() const
	{
		return { x * 0.5f, y * 0.5f, z * 0.5f, 1.0f };
	}

	inline float Len2D() const
	{
		return sqrtf((x * x) + (y * y));
	}

	inline void Normal2D()
	{
		float Len = Len2D();
		if (0 == Len)
		{
			return;
		}

		x /= Len;
		y /= Len;
		return;
	}

	inline void Range2D(float _Max)
	{
		Normal2D();

		x *= _Max;
		y *= _Max;
		return;
	}

	inline void Limit2D(float _Max)
	{
		if (_Max <= Len2D())
		{
			Range2D(_Max);
		}
		
		return;
	}

	// 보간 필요한것같아서 제작
	static float4 Lerp(float4 _Start, float4 _Goal, float _Value)
	{
		if (1 <= _Value)
		{
			_Value = 1;
		}
		if (0 >= _Value)
		{
			_Value = 0;
		}

		return _Start * (1 - _Value) + _Goal * _Value;
	}
	// 보간 필요한것같아서 제작
	static float LerpFloat(float _Start, float _Goal, float _Value)
	{
		if (1 <= _Value)
		{
			_Value = 1;
		}
		if (0 >= _Value)
		{
			_Value = 0;
		}

		return _Start * (1 - _Value) + _Goal * _Value;
	}

	float4 operator-(const float4& _Other) const
	{
		return { x - _Other.x, y - _Other.y, z - _Other.z, 1.0f };
	}
	float4 operator-() const
	{
		return { -x, -y, -z, 1.0f };
	}
	float4 operator+(const float4& _Other) const
	{
		return { x + _Other.x, y + _Other.y, z + _Other.z, 1.0f };
	}
	float4 operator*(const float _Value) const
	{
		return { x * _Value, y * _Value, z * _Value, 1.0f };
	}
	float4& operator+=(const float4& _Other)
	{
		x += _Other.x;
		y += _Other.y;
		z += _Other.z;

		return *this;
	}
	float4& operator-=(const float4& _Other)
	{
		x -= _Other.x;
		y -= _Other.y;
		z -= _Other.z;

		return *this;
	}
	float4& operator*=(const float _Other)
	{
		x *= _Other;
		y *= _Other;
		z *= _Other;

		return *this;
	}
	float4& operator*=(const float4& _Other)
	{
		x *= _Other.x;
		y *= _Other.y;
		z *= _Other.z;

		return *this;
	}
	

	bool CompareInt2D(const float4& _Value) const
	{
		return ix() == _Value.ix() && iy() == _Value.iy();
	}

	bool CompareInt3D(const float4& _Value) const
	{
		return ix() == _Value.ix() &&
			iy() == _Value.iy() &&
			iz() == _Value.iz();
	}

	float4 RotationToDegreeZ(float _Degree)
	{
		return RotationToRadianZ(_Degree * GameEngineMath::DegreeToRadian);
	}

	float4 RotationToRadianZ(float _Radian)
	{
		*this = VectorRotationToRadianZ(*this, _Radian);
		return *this;
	}

	POINT ToWinAPIPOINT() const
	{
		POINT NewPoint;
		NewPoint.x = ix();
		NewPoint.y = iy();
		return NewPoint;
	}

public:
	float4()
		: x(0)
		, y(0)
		, z(0)
		, w(1.0f)
	{
	}
	float4(float _x, float _y)
		: x(_x)
		, y(_y)
		, z(0)
		, w(1.0f)
	{
	}
	float4(float _x, float _y, float _z)
		: x(_x)
		, y(_y)
		, z(_z)
		, w(1.0f)
	{
	}
	float4(float _x, float _y, float _z, float _w)
		: x(_x)
		, y(_y)
		, z(_z)
		, w(_w)
	{
	}
};

class GameEngineRect
{
public:
	float4 Pos;
	float4 Scale;

public:
	float4 CenterLeftTopPoint() const
	{
		return { static_cast<float>(CenterLeft()), static_cast<float>(CenterTop()) };
	}

	float4 CenterLeftBotPoint() const
	{
		return { static_cast<float>(CenterLeft()), static_cast<float>(CenterBot()) };
	}

	float4 CenterRightTopPoint() const
	{
		return { static_cast<float>(CenterRight()), static_cast<float>(CenterTop()) };
	}

	float4 CenterRightBotPoint() const
	{
		return { static_cast<float>(CenterRight()), static_cast<float>(CenterBot()) };
	}

public:
	inline int CenterLeft() const
	{
		return Pos.ix() - Scale.hix();
	}
	inline int CenterRight() const
	{
		return Pos.ix() + Scale.hix();
	}
	inline int CenterTop() const
	{
		return Pos.iy() - Scale.hiy();
	}
	inline int CenterBot() const
	{
		return Pos.iy() + Scale.hiy();
	}

	inline bool OverLap(const GameEngineRect& _Other)
	{
		if (CenterBot() < _Other.CenterTop())
		{
			return false;
		}

		if (CenterTop() > _Other.CenterBot())
		{
			return false;
		}

		if (CenterRight() < _Other.CenterLeft())
		{
			return false;
		}

		if (CenterLeft() > _Other.CenterRight())
		{
			return false;
		}

		return true;
	}

public:
	GameEngineRect(float4 _Pos, float4 _Scale)
		: Pos(_Pos)
		, Scale(_Scale)
	{
	}
};