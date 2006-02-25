include Makefile.conf

SUBDIRS = src/ src/libs/ src/tools/ src/client/


DIRR = bartlby-core bartlby-plugins bartlby-php bartlby-ui bartlby-docs

all:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && make all); \
	done
	
install: all
	$(MKDIRP) ${BARTLBY_HOME}/bin/
	$(MKDIRP) ${BARTLBY_HOME}/etc/
	$(MKDIRP) ${BARTLBY_HOME}/lib/
	$(MKDIRP) ${BARTLBY_HOME}/var/
	$(MKDIRP) ${BARTLBY_HOME}/perf/
	$(MKDIRP) ${BARTLBY_HOME}/trigger/
	
	
	$(CPPVA) bartlby.cfg ${BARTLBY_HOME}
	$(CPPVA) bartlby.startup ${BARTLBY_HOME}
	$(CHMOD) a+x ${BARTLBY_HOME}/bartlby.startup 
	$(CPPVA) trigger/* ${BARTLBY_HOME}/trigger/
	$(CPPVA) perf/* ${BARTLBY_HOME}/perf/
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && make install); \
	done


clean:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && make clean); \
	done
changelog:
	../make_changelog
	
	
	
cvs-clean:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && make clean); \
	done
	$(RMVFR) *.so
	$(RMVFR) bartlby
	$(RMVFR) shmt
	$(RMVFR) bartlby_shmt
	$(RMVFR) bartlby_agent
	$(RMVFR) bartlby_cmd
	$(RMVFR) bartlby_portier
	$(RMVFR) *2006*
	$(RMVFR) config.status
	$(RMVFR) config.log
	$(RMVFR) autom4te.cache/
	$(RMVFR) bartlby.cperf
	$(RMVFR) bartlby.cfg
	$(RMVFR) Makefile.conf
	
	
	
	
	
	
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

