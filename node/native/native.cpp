#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#ifndef XMLSEC_NO_XSLT
#include <libxslt/xslt.h>
#include <libxslt/security.h>
#endif /* XMLSEC_NO_XSLT */

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/xmlenc.h>
#include <xmlsec/crypto.h>

#include <node.h>

#include <iostream>

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
#ifndef XMLSEC_NO_XSLT
	xsltSecurityPrefsPtr xsltSecPrefs = NULL;
#endif /* XMLSEC_NO_XSLT */

	/* Init libxml and libxslt libraries */
	xmlInitParser();
	LIBXML_TEST_VERSION
		xmlLoadExtDtdDefaultValue = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
	xmlSubstituteEntitiesDefault(1);
#ifndef XMLSEC_NO_XSLT
	xmlIndentTreeOutput = 1;
#endif /* XMLSEC_NO_XSLT */

	/* Init libxslt */
#ifndef XMLSEC_NO_XSLT
	/* disable everything */
	xsltSecPrefs = xsltNewSecurityPrefs();
	xsltSetSecurityPrefs(xsltSecPrefs,  XSLT_SECPREF_READ_FILE,        xsltSecurityForbid);
	xsltSetSecurityPrefs(xsltSecPrefs,  XSLT_SECPREF_WRITE_FILE,       xsltSecurityForbid);
	xsltSetSecurityPrefs(xsltSecPrefs,  XSLT_SECPREF_CREATE_DIRECTORY, xsltSecurityForbid);
	xsltSetSecurityPrefs(xsltSecPrefs,  XSLT_SECPREF_READ_NETWORK,     xsltSecurityForbid);
	xsltSetSecurityPrefs(xsltSecPrefs,  XSLT_SECPREF_WRITE_NETWORK,    xsltSecurityForbid);
	xsltSetDefaultSecurityPrefs(xsltSecPrefs);
#endif /* XMLSEC_NO_XSLT */


	/* Init xmlsec library */
	if(xmlSecInit() < 0) {
		cout << "Error: xmlsec initialization failed." << endl;
		return;
	}

	/* Check loaded library version */
	if(xmlSecCheckVersion() != 1) {
		cout << "Error: loaded xmlsec library version is not compatible." << endl;
		return;
	}

	/* Load default crypto engine if we are supporting dynamic
	 * loading for xmlsec-crypto libraries. Use the crypto library
	 * name ("openssl", "nss", etc.) to load corresponding
	 * xmlsec-crypto library.
	 * */
#ifdef XMLSEC_CRYPTO_DYNAMIC_LOADING
	if(xmlSecCryptoDLLoadLibrary(NULL) < 0) {
		cout << "Error: unable to load default xmlsec-crypto library. Make sure" << endl;
		cout << "that you have it installed and check shared libraries path" << endl;
		cout << "(LD_LIBRARY_PATH) envornment variable." << endl;
		return;
	}
#endif /* XMLSEC_CRYPTO_DYNAMIC_LOADING */

	/* Init crypto library */
	if(xmlSecCryptoAppInit(NULL) < 0) {
		cout << "Error: crypto initialization failed." << endl;
		return;
	}

	/* Init xmlsec-crypto library */
	if(xmlSecCryptoInit() < 0) {
		cout << "Error: xmlsec-crypto initialization failed." << endl;
		return;
	}


	/* Shutdown xmlsec-crypto library */
	xmlSecCryptoShutdown();

	/* Shutdown crypto library */
	xmlSecCryptoAppShutdown();

	/* Shutdown xmlsec library */
	xmlSecShutdown();

	/* Shutdown libxslt/libxml */
#ifndef XMLSEC_NO_XSLT
	xsltCleanupGlobals();
#endif /* XMLSEC_NO_XSLT */
	xmlCleanupParser();

	args.GetReturnValue().Set(String::NewFromUtf8(isolate, "Hello from native module"));
}

void init(Local<Object> exports) {
	NODE_SET_METHOD(exports, "hello", Method);
}

NODE_MODULE(addon, init)
