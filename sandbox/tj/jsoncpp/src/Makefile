INCLUDE = -I../include
CC=/usr/bin/g++
OBJ=json_reader.o json_writer.o json_value.o

json_test:	json_test.cpp  $(OBJ)
	$(CC) $(INCLUDE) -o $@ $< $(OBJ)

jsoncpp.so:	json_reader.o json_writer.o json_value.o
	$(CC) -shared -o $@ $?

json_reader.o:	json_reader.cpp
	$(CC) $(INCLUDE) -c -o $@ $<

json_writer.o:	json_writer.cpp
	$(CC) $(INCLUDE) -c -o $@ $<

json_value.o:	json_value.cpp
	$(CC) $(INCLUDE) -c -o $@ $<

all:	jsoncpp.so $(OBJ) json_test

clean:
	-rm *.so $(OBJ) json_test

# DO NOT DELETE from makedepend

json_reader.o: ../include/json/reader.h ../include/json/forwards.h
json_reader.o: ../include/json/config.h ../include/json/value.h
json_value.o: ../include/json/writer.h ../include/json/value.h
json_value.o: ../include/json/forwards.h ../include/json/config.h
json_value.o: json_batchallocator.h /usr/include/stdlib.h
json_value.o: json_valueiterator.inl
json_writer.o: ../include/json/writer.h ../include/json/value.h
json_writer.o: ../include/json/forwards.h ../include/json/config.h
