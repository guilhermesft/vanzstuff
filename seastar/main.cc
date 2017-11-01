#include "core/app-template.hh"
#include "core/future-util.hh"
#include "core/future.hh"
#include "core/reactor.hh"
#include "core/seastar.hh"
#include "core/sleep.hh"
#include "util/log.hh"

#include <iostream>
#include <stdexcept>
	
using namespace std::chrono_literals;

// prototypes
seastar::future<> service_loop();
seastar::future<> slow_op();
seastar::future<int> fast();
seastar::future<int> incr(int i);

seastar::future<> service_loop()
{
	return slow_op().then( [] {
		std::cout << "Done!\n";
	});
}

seastar::future<int> fast()
{
	return seastar::make_ready_future<int>(3);
}

seastar::future<> slow_op() 
{
	return seastar::sleep(1s);
}

seastar::future<int> incr(int i)
{
	return seastar::sleep(10ms).then([i] { return i + 1; });
}

extern seastar::future<> f() 
{
	return seastar::parallel_for_each(boost::irange<unsigned>(0, seastar::smp::count),
		[] (unsigned c) {
			// run the given function (service_loop) in a core
			return seastar::smp::submit_to(c, service_loop);
	});
}

int main(int argc, char** argv) {
	seastar::app_template app;
	try {
		app.run(argc, argv, f);
	} catch (...) {
		std::cerr << "Couldn't start application: "
			<< std::current_exception() << std::endl;
		return 1;
	}
	return 0;
}
