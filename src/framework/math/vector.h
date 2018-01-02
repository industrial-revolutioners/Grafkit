#pragma once 
#include "../render/dxtypes.h"

namespace Grafkit {

	class Vector {
	public:
		inline static float2 Normalize(float2 v) {
			auto xv = DirectX::XMLoadFloat2(&v);
			auto xRes = DirectX::XMVector2Normalize(xv);
			float2 res;
			DirectX::XMStoreFloat2(&res, xRes);
			return res;
		}

		inline static float3 Normalize(float3 v) {
			auto xv = DirectX::XMLoadFloat3(&v);
			auto xRes = DirectX::XMVector3Normalize(xv);
			float3 res;
			DirectX::XMStoreFloat3(&res, xRes);
			return res;
		}

		inline static float4 Normalize(float4 v) {
			auto xv = DirectX::XMLoadFloat4(&v);
			auto xRes = DirectX::XMVector4Normalize(xv);
			float4 res;
			DirectX::XMStoreFloat4(&res, xRes);
			return res;
		}
	};

};

inline float3 operator+ (const float3 &a, const float3 &b) { return float3(a.x + b.x, a.y + b.y, a.z + b.z); }
inline float3 operator- (const float3 &a, const float3 &b) { return float3(a.x - b.x, a.y - b.y, a.z - b.z); }

inline float3 operator+ (const float3 &a, const float &b) { return float3(a.x + b, a.y + b, a.z + b); }
inline float3 operator+ (const float3 &a, const double &b) { return float3(a.x + b, a.y + b, a.z + b); }

inline float3 operator- (const float3 &a, const float &b) { return float3(a.x - b, a.y - b, a.z - b); }
inline float3 operator- (const float3 &a, const double &b) { return float3(a.x - b, a.y - b, a.z - b); }

inline float3 operator* (const float3 &a, const float3 &b) { return float3(a.x * b.x, a.y * b.y, a.z * b.z); }
inline void operator*= (float3 &a, const float3 &b) { a = float3(a.x * b.x, a.y * b.y, a.z * b.z); }

inline float3 operator* (const float3 &a, const float &b) { return float3(a.x * b, a.y * b, a.z * b); }
inline float3 operator* (const float3 &a, const double &b) { return float3(a.x * b, a.y * b, a.z * b); }
inline void operator*= (float3 &a, const float &b) { a = float3(a.x * b, a.y * b, a.z * b); }
inline void operator*= (float3 &a, const double &b) { a = float3(a.x * b, a.y * b, a.z * b); }


inline float4 operator+ (const float4 &a, const float4 &b) { return float4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
inline float4 operator- (const float4 &a, const float4 &b) { return float4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }

inline float4 operator+ (const float4 &a, const float &b) { return float4(a.x + b, a.y + b, a.z + b, a.w + b); }
inline float4 operator+ (const float4 &a, const double &b) { return float4(a.x + b, a.y + b, a.z + b, a.w + b); }

inline float4 operator- (const float4 &a, const float &b) { return float4(a.x - b, a.y - b, a.z - b, a.w - b); }
inline float4 operator- (const float4 &a, const double &b) { return float4(a.x - b, a.y - b, a.z - b, a.w - b); }

inline float4 operator* (const float4 &a, const float4 &b) { return float4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
inline void operator*= (float4 &a, const float4 &b) { a = float4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }

inline float4 operator* (const float4 &a, const float &b) { return float4(a.x * b, a.y * b, a.z * b, a.w * b); }
inline float4 operator* (const float4 &a, const double &b) { return float4(a.x * b, a.y * b, a.z * b, a.w * b); }
inline void operator*= (float4 &a, const float &b) { a = float4(a.x * b, a.y * b, a.z * b, a.w * b); }
inline void operator*= (float4 &a, const double &b) { a = float4(a.x * b, a.y * b, a.z * b, a.w * b); }
