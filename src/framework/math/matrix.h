#pragma once
#include "../render/dxtypes.h"
#include "quaternion.h"
#include "../utils/memory.h"

namespace Grafkit {
	/**
	Extends DirectX::XMMATRIX with operations to make thing easier, and use as a real type.
	https://msdn.microsoft.com/en-us/library/windows/desktop/ee415594(v=vs.85).aspx

	NOTE THIS FUCKER ALL THE TIME http://stackoverflow.com/questions/11285680/crash-after-m-xmmatrixidentity-aligment-memory-in-classes

	`::matrix` is a shortcut to `DirectX::XMMATRIX`
	*/

	__declspec(align(16)) class Matrix : public AlignedNew<Matrix>
	{

	private:
		matrix mat;

	public:
		// --- constructors
		Matrix() {
			this->Identity();
		}

		Matrix(
			float a1, float a2, float a3, float a4,
			float b1, float b2, float b3, float b4,
			float c1, float c2, float c3, float c4,
			float d1, float d2, float d3, float d4
		) {
			this->mat = DirectX::XMMatrixSet(
				a1, a2, a3, a4,
				b1, b2, b3, b4,
				c1, c2, c3, c4,
				d1, d2, d3, d4
			);
		}

		Matrix(const matrix &m) : mat(m) {
		}

		Matrix(const Matrix &m) : mat(m.mat) {
		}

		Matrix(const Quaternion &q) {
			mat = (matrix)q;
		}

		// identity
		void Identity() {
			this->mat = DirectX::XMMatrixIdentity();
		}

		///@{
		/// Multiply
		void Multiply(const matrix &m)
		{
			this->mat = DirectX::XMMatrixMultiply(m, this->mat);
		}

		void Multiply(const Matrix &m)
		{
			this->mat = DirectX::XMMatrixMultiply(m.mat, this->mat);
		}
		///@}

		///@{
		///translation
		void Translate(const float3 &v)
		{
			this->mat = DirectX::XMMatrixMultiply(this->mat, DirectX::XMMatrixTranslation(v.x, v.y, v.z));
		}

		void Translate(float x, float y, float z)
		{
			this->mat = DirectX::XMMatrixMultiply(this->mat, DirectX::XMMatrixTranslation(x, y, z));
		}
		///@}

		/// banana for
		void Scale(const float3 &v)
		{
			this->mat = DirectX::XMMatrixMultiply(this->mat, DirectX::XMMatrixScaling(v.x, v.y, v.z));
		}

		void Scale(float x, float y, float z)
		{
			this->mat = DirectX::XMMatrixMultiply(this->mat, DirectX::XMMatrixScaling(x, y, z));
		}

		/** Rotate around a given axis
			@param v given axis
			@param phi angle, rad */
		void Rotate(const float3 &v, float phi)
		{
			dxvector vv;
			vv = DirectX::XMLoadFloat3(&v);
			this->mat = DirectX::XMMatrixMultiply(this->mat, DirectX::XMMatrixRotationNormal(vv, phi));
		}

		void Rotate(const Quaternion& q) {
			Multiply((matrix)q);
		}

		/// @todo https://en.wikipedia.org/wiki/Euler_angles

		/// Roll-Pitch-Yaw rotation, in rad
		void RotateRPY(float roll, float pitch, float yaw)
		{
			this->mat = DirectX::XMMatrixMultiply(this->mat, DirectX::XMMatrixRotationRollPitchYaw(roll, pitch, yaw));
		}

		// --- lookat
		void LookAtLH(const float3 &e, const float3 &c = float3(0,0,0), const float3 & u = float3(0, 1, 0)) {
			dxvector ev = XMLoadFloat3(&e);
			dxvector cv = XMLoadFloat3(&c);
			dxvector uv = XMLoadFloat3(&u);
			mat = DirectX::XMMatrixLookAtLH(ev, cv, uv);
			//Invert(); // something goez wrong w/ the matrix
		}

		void LookAtRH(const float3 &e, const float3 &c = float3(), const float3 & u = float3(0, 1, 0)) {
			dxvector ev = XMLoadFloat3(&e);
			dxvector cv = XMLoadFloat3(&c);
			dxvector uv = XMLoadFloat3(&u);
			mat = DirectX::XMMatrixLookAtRH(ev, cv, uv);
			//Invert(); // something goez wrong w/ the matrix
		}

		/// Transpose matrix
		void Transpose() {
			mat = DirectX::XMMatrixTranspose(mat);
		}

		/// Invert matrix
		void Invert() {
			mat = DirectX::XMMatrixInverse(nullptr, mat);
		}

		// ===
		// --- operations

		void Set(const Matrix& m) { mat = m.mat; }
		void Set(const matrix& m) { mat = m; }

		/// @note operator= valamiert nem mukodik, kiveve, ha ... 
		Matrix& operator= (const Matrix& m)
		{
			mat = m.mat;
			return *this;
		}

		Matrix& operator= (const matrix &m)
		{
			mat = m;
			return *this;
		}

		// --- getters
		const matrix& Get() const {
			return mat;
		}

		const float Get(size_t row, size_t col) const {
#ifdef _XM_NO_INTRINSICS_
			return mat.r[row][col];
#else
			return DirectX::XMVectorGetByIndex(mat.r[row], col);
#endif 

		}

		// --- matrix - vector ops
		float4 Transfrom(float4 &f4) const {
			dxvector v = XMVector4Transform(XMLoadFloat4(&f4), mat);
			float4 r;
			XMStoreFloat4(&r, v);
			return r;
		}

		float3 Transfrom(float3 &f3) const {
			dxvector v = XMVector3Transform(XMLoadFloat3(&f3), mat);
			float3 r;
			XMStoreFloat3(&r, v);
			return r;
		}
	};
}