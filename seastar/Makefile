
all:
	ninja -C build -v

clean:
	rm -rf build

release: clean
	meson --buildtype=plain build

docker:
	docker build -t seastar_demo:latest .



