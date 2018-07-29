pluma_VERSION=2.4.2

all:
	$(MAKE) -C bubble_sort pluma_VERSION=$(pluma_VERSION)
	$(MAKE) -C bubble_sort1 pluma_VERSION=$(pluma_VERSION)
	$(MAKE) -C bubble_sort2 pluma_VERSION=$(pluma_VERSION)
	$(MAKE) -C quick_sort_last pluma_VERSION=$(pluma_VERSION)
	$(MAKE) -C quick_sort_middle pluma_VERSION=$(pluma_VERSION)
	$(MAKE) -C my_sort pluma_VERSION=$(pluma_VERSION)

clean:
	$(MAKE) -C bubble_sort clean
	$(MAKE) -C bubble_sort1 clean
	$(MAKE) -C bubble_sort2 clean
	$(MAKE) -C quick_sort_last clean
	$(MAKE) -C quick_sort_middle clean
	$(MAKE) -C my_sort clean

.PHONY: all clean

