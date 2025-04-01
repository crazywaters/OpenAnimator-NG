.PHONY: all release debug run poco clean

all: debug run

debug:
	rm -rf build && \
	mkdir -p _build && \
	cd _build && \
	cmake -G "Ninja" -DWITH_POCO=OFF .. && \
	cmake --build . --config debug --target install && \
	cd ..

release:
	rm -rf _build && \
	mkdir -p _build && \
	cd _build && \
	cmake -G "Ninja" -DWITH_POCO=OFF .. && \
	cmake --build . --config RelWithDebInfo --target install && \
	cd ..

run:
	@cd _build/bin &&\
	./ani

lldb:
	@cd _build/bin && lldb ani --batch \
		 --one-line 'process launch' \
		 --one-line-on-crash 'bt' \
	     --one-line-on-crash 'quit'

poco:
	_build/bin/poco

clean:
	rm -rf _build && \
	echo Project Cleaned

