#include "../stdafx.h"
#include "ThreadPool.h"

using namespace Util::Work;

void CALLBACK DefaultWorkApplicationFunction(PTP_CALLBACK_INSTANCE instance, void * context, PTP_WORK work)
{
	Util::Work::WorkItem * wi = reinterpret_cast<Util::Work::WorkItem*>(context);
	wi->func(wi->data);
	delete wi;
	CloseThreadpoolWork(work);
}

ThreadPool::ThreadPool() { };

ThreadPool::~ThreadPool()
{
	CloseThreadpoolCleanupGroupMembers(mCleanGroup, false, nullptr);
	CloseThreadpoolCleanupGroup(mCleanGroup);
	CloseThreadpool(mPool);
}

void ThreadPool::initalize( int nbrThreads )
{
	if( nbrThreads == 0 )
		nbrThreads = getMaxNumberOfHardwareThreads();
	
	// initalize the private thread pool environment 
	InitializeThreadpoolEnvironment(&mEnviro);
	
	// Create the pool
	mPool = CreateThreadpool( 0 );
	
	// Initalize the thread pool with the same number of threads as specified.
	// Setting min==max ensures the pool will keep the threads around. 
	SetThreadpoolThreadMaximum(mPool, nbrThreads);
	SetThreadpoolThreadMinimum(mPool, nbrThreads);

	// Create the cleanup group
	mCleanGroup = CreateThreadpoolCleanupGroup();

	// Set the call back pool
	SetThreadpoolCallbackPool(&mEnviro, mPool);
	
	// Set the cleanup group
	SetThreadpoolCallbackCleanupGroup(&mEnviro, mCleanGroup, nullptr);
}

void Util::Work::ThreadPool::submitWork(WorkItem item)
{
	PTP_WORK work = CreateThreadpoolWork(DefaultWorkApplicationFunction,
		reinterpret_cast<void*>(new WorkItem(item)),
		&mEnviro);

	SubmitThreadpoolWork(work);
}


int Util::Work::getMaxNumberOfHardwareThreads()
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo( &sysinfo );

	return sysinfo.dwNumberOfProcessors;
}


