all:
	bash scripts/check-cmakelists.sh
	@for d in `find src -type d | sort`; do \
		if [ -f "$$d/Makefile" ]; then \
			echo "[Test] $$d"; \
			make -C $$d || exit 1; \
		fi; \
	done

clean:
	@for d in `find src -type d | sort`; do \
		if [ -f "$$d/Makefile" ]; then \
			make -C $$d clean; \
		fi; \
	done
