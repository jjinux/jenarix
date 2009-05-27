
include .config

PRIME = ls *.[cm]* | sed 's/\.c$$/.o/;s/\.cpp$$/.o/;s/\.m$$/.o/'| awk 'BEGIN{printf("OBJS=")}{printf("%s ",$$1)}END{print}'>.files;ls *.[cm]* | sed 's/\.c$$/.p/;s/\.cpp$$/.p/;s/\.m$$/.p/'| awk 'BEGIN{printf("DEPS=")}{printf("%s ",$$1)}END{print}'>>.files

#; touch .depends; cat .files .depends 
#> .includes

src/.files:
	cd src;$(PRIME)

