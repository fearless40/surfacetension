class D3D11;

namespace Test {
		

	class MicroManagerTest {

		struct InternalData;
		
	private:
		InternalData *	mData;
		D3D11 &			md3d;
		bool			mIsOk;
		void clearData();

	public:
		MicroManagerTest( D3D11 & d3d );
		~MicroManagerTest();
		



		bool isOk(); 
		
		bool initalize(float width, float height);

		bool frame();

	};
};