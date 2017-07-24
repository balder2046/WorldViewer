#pragma once

namespace TRN
{
	class Patch
	{
	public:

		Patch(int iPatchX,int iPatchY);
		void Draw();
	public:
		void Prepare();
	private:
		int m_iPatchX;
		int m_iPatchY;
		int m_iTextureIndex;
	};

	class Terrain
	{
	public:

		Terrain();
	public:
		void Init();
		void Draw();
	private:
		float centerx;
		float centery;
		float centerz;
	private:
		float m_fPatchSize;
	public:
		void toLocalPos(float &x,float &y, float &z);
		void toWorldPos(float &x, float &y, float &z);
	public:
		float getCenterX();
		float getCenterY();


	};

}
