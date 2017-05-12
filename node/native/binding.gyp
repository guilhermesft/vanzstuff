{
	"targets": [
	{
		"target_name": "xmlsecnode",
		"sources": [ "native.cpp" ],
		"include_dirs": [
			"/usr/include/libxml2/",
			"/usr/include/xmlsec1/",
		],
		"defines": ["XMLSEC_CRYPTO_OPENSSL"],
		"link_settings": {
			"libraries": ['-lxmlsec1'],
		},
	}]
}
