{
	"targets": [
	{
		"target_name": "xmlsec",
		"sources": [ "native.cpp" ],
		"cflags": [ "-lxmlsec1" ],
			"include_dirs": [
			"/usr/include/libxml2/",
			"/usr/include/xmlsec1/",
			],
		"defines": ["XMLSEC_CRYPTO_OPENSSL"]
	}]
}
