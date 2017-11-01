#include "core/app-template.hh"
#include "core/future.hh"
#include "core/sleep.hh"
#include "util/log.hh"

#include <iostream>
#include <stdexcept>

seastar::future<int> fast()
{
	return seastar::make_ready_future<int>(3);
}

seastar::future<int> incr(int i)
{
	using namespace std::chrono_literals;
	return seastar::sleep(10ms).then([i] { return i + 1; });
}

extern seastar::future<> f() 
{
	return incr(3).then([] (int val) {
		std::cout << "Got " << val << std::endl;
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
