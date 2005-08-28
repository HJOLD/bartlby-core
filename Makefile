SUBDIRS = src/ src/libs/ src/tools/ src/client/

all:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && make all); \
	done


clean:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && make clean); \
	done
changelog:
	cd /storage/SF.NET/BARTLBY/bartlby-plugins
	cvs2cl.pl
	cd /storage/SF.NET/BARTLBY/bartlby-ui
	cvs2cl.pl
	cd /storage/SF.NET/BARTLBY/bartlby-plugins
	cvs2cl.pl
	cd /storage/SF.NET/BARTLBY/bartlby
	cvs2cl.pl
	
	
	
cvs-clean: changelog
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && make clean); \
	done
	rm -f *.so
	rm -f bartlby
	rm -f shmt
	rm -f bartlby_agent
	
cvs: cvs-clean
	cvs commit;
	
cvs-single: cvs-clean
		
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  for x in $$subdir*.c; do \
	  	cvs commit $$x; \
	  done \
	done
	
sf-release: 
	./makedist
	echo "goto admin page and add release"

website: changelog
	
	scp ChangeLog hjanuschka@shell.sourceforge.net:/home/users/h/hj/hjanuschka/bartlby/htdocs/ChangeLog
	
