SUBDIRS = src/ src/libs/ src/tools/ src/client/
INST_DIR = /home/bartlby/

DIRR = bartlby-core bartlby-plugins bartlby-php bartlby-ui bartlby-docs

all:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && make all); \
	done


clean:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && make clean); \
	done
changelog:
	../make_changelog
	
	
	
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
	../makedist
	echo "goto admin page and add release"

website: changelog 
	
	scp ../CHANGELOG hjanuschka@shell.sourceforge.net:/home/users/h/hj/hjanuschka/bartlby/htdocs/ChangeLog
	list='$(DIRR)'; for subdir in $$list; do \
	  for x in $$subdir; do \
	  	scp /storage/SF.NET/BARTLBY/$$x/ChangeLog hjanuschka@shell.sourceforge.net:/home/users/h/hj/hjanuschka/bartlby/htdocs/ChangeLog-$$x; \
	  done \
	done
	date > /storage/SF.NET/BARTLBY/lastMod
	scp /storage/SF.NET/BARTLBY/lastMod hjanuschka@shell.sourceforge.net:/home/users/h/hj/hjanuschka/bartlby/htdocs/lastMod

install: all
	cp bartlby ${INST_DIR} 
	cp bartlby-dev.cfg ${INST_DIR}
	chown -R bartlby ${INST_DIR}/
