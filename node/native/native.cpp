#include <iostream>

#include <node.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/xmlenc.h>
#include <xmlsec/crypto.h>

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

using std::cout;
using std::endl;


void Method(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	/* Init xmlsec library */
	if(xmlSecInit() < 0) {
		cout << "Error: xmlsec initialization failed" << endl;
		return;
	}

	/* Check loaded library version */
	if(xmlSecCheckVersion() != 1) {
		cout << "Error: loaded xmlsec library version is not compatible" << endl;
		return;
	}

	/* Init crypto library */
	if(xmlSecCryptoAppInit(NULL) < 0) {
		cout << "Error: crypto initialization failed" << endl;
		return;
	}

	/* Init xmlsec-crypto library */
	if(xmlSecCryptoInit() < 0) {
		cout << "Error: xmlsec-crypto initialization failed" << endl;
		return;
	}
	args.GetReturnValue().Set(String::NewFromUtf8(isolate, "hello world"));
}


void Shutdown(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	/* Shutdown xmlsec-crypto library */
	xmlSecCryptoShutdown();

	/* Shutdown crypto library */
	xmlSecCryptoAppShutdown();

	/* Shutdown xmlsec library */
	xmlSecShutdown();
}

void init(Local<Object> exports) {
	NODE_SET_METHOD(exports, "hello", Method);
	NODE_SET_METHOD(exports, "shutdown", Shutdown);
}

NODE_MODULE(addon, init)
