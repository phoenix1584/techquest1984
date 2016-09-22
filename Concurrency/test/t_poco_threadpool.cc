/*
 * Copyright (c) 2016
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#define BOOST_TEST_MODULE  POCO_THREAD 
#include <boost/test/unit_test.hpp>

/*
* Objective
* Explore POCO thread pool as a potential candidate for worker thread pool for services.
*/
#include "Poco/ThreadPool.h"
#include "Poco/RunnableAdapter.h"
#include "Poco/Exception.h"
#include "Poco/Thread.h"
#include "Poco/Mutex.h"

class Worker{
    private:
        int m_sleep;
        Poco::FastMutex m_mutex;
    public:
        Worker(int interval)
            :m_sleep(interval)
        {}
        void TaskNoArgs(){
            {
                Poco::FastMutex::ScopedLock lock(m_mutex);
                BOOST_TEST_MESSAGE("Invoked with sleep interval of " << m_sleep );
            }
            sleep(m_sleep);
        }
};

void BoostCheckHelper(const Poco::ThreadPool& test_pool ,int allocated, int used , int capacity, int available){
	BOOST_CHECK (test_pool.allocated() == allocated);
	BOOST_CHECK (test_pool.used() == used);
	BOOST_CHECK (test_pool.capacity() == capacity);
	BOOST_CHECK (test_pool.available() == available);
    BOOST_CHECK ( capacity == (used+available));
}

BOOST_AUTO_TEST_SUITE(poco_thread)
BOOST_AUTO_TEST_CASE(basic_pool){
    Poco::ThreadPool test_pool(2,3,10);
    test_pool.setStackSize(1);
    BoostCheckHelper(test_pool,2,0,3,3);
	test_pool.addCapacity(1);
    BoostCheckHelper(test_pool,2,0,4,4);
}

BOOST_AUTO_TEST_CASE(thread_work_noargs){
    Poco::ThreadPool test_pool(2,3,5);
    test_pool.setStackSize(1);
    Worker test_worker(4);
    Poco::RunnableAdapter<Worker> run_adapter(test_worker, &Worker::TaskNoArgs);
    test_pool.start(run_adapter);
    BoostCheckHelper(test_pool,2,1,3,2);
    
    Poco::RunnableAdapter<Worker> run_adapter1(test_worker, &Worker::TaskNoArgs);
    test_pool.start(run_adapter1);
    BoostCheckHelper(test_pool,2,2,3,1);
    
    Worker test_worker2(2);
    Poco::RunnableAdapter<Worker> run_adapter2(test_worker2, &Worker::TaskNoArgs);
    test_pool.start(run_adapter2);
    BoostCheckHelper(test_pool,3,3,3,0);
	test_pool.joinAll();
}

BOOST_AUTO_TEST_CASE(pool_timeout){
    Poco::ThreadPool test_pool(2,5,5);
    test_pool.setStackSize(1);
    BoostCheckHelper(test_pool,2,0,5,5);
    Worker test_worker(20);
    Worker test_worker_extra(5);
    Poco::RunnableAdapter<Worker> run_adapter(test_worker, &Worker::TaskNoArgs);
    Poco::RunnableAdapter<Worker> run_adapter_extra(test_worker_extra, &Worker::TaskNoArgs);
    test_pool.start(run_adapter_extra);
    test_pool.start(run_adapter_extra);
    sleep(3);
    test_pool.start(run_adapter);
    test_pool.start(run_adapter);
    test_pool.start(run_adapter);
    sleep(3);
    BoostCheckHelper(test_pool,5,3,5,2);
	test_pool.joinAll();

    for(int i = 0 ; i < 10000 ; ++i )
        usleep(1000);
    test_pool.start(run_adapter_extra);
    sleep(3);
    BoostCheckHelper(test_pool,3,1,5,4);
	test_pool.joinAll();
    sleep(3);
    BoostCheckHelper(test_pool,2,0,5,5);
}

BOOST_AUTO_TEST_CASE(thread_unavailable){
    Poco::ThreadPool test_pool(2,3,5);
    test_pool.setStackSize(1);
    Worker test_worker(10);
    Poco::RunnableAdapter<Worker> run_adapter(test_worker, &Worker::TaskNoArgs);
    try{
        test_pool.start(run_adapter);
        test_pool.start(run_adapter);
        test_pool.start(run_adapter);
        test_pool.start(run_adapter);
    }catch(Poco::NoThreadAvailableException& e){
        BOOST_TEST_MESSAGE( "[Exception] : " << e.what());
    }
	catch (...){
		BOOST_ASSERT( 0 && "wrong exception thrown");
	}
}

BOOST_AUTO_TEST_SUITE_END()



