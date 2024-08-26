#include <atomic>
#include "Work.h"

namespace Util
{
	namespace Work
	{
		

		class ThreadPool
		{
		private:
			PTP_POOL mPool;
			TP_CALLBACK_ENVIRON mEnviro;
			PTP_CLEANUP_GROUP mCleanGroup;

		public:
			ThreadPool();
			~ThreadPool();

			void initalize(int nbrThreads = 0);

			void submitWork(WorkItem item);



		};

		int getMaxNumberOfHardwareThreads();

		

		


	};
}