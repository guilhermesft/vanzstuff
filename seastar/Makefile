
all:
	ninja -C build -v

clean:
	rm -rf build

release: clean
	meson --buildtype=plain build

run: 
	./build/seastar_demo -c4 -m1G

docker:
	docker build -t seastar_demo:latest .



