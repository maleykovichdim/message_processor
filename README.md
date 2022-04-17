# message_processor


# Project description:
	development envirounment: C++17, Visual Studio 2019, Windows 10
	
	## The main idea:

	### Parsers ask for permission to parse from a fast atomic, when the counter overflows,
	then data is saving into the storage and then to the file, after that the time delay is calculated and applied if it is needed

	
	
	## files:
	   /generator
	     .... generator.cpp     Project for creation of test data files with messages: 
		                        no gaps in the sequence, there are repetitions, the data is mixed.
								Sequence from 1 to 7000.
								you can use it for following types: unsigned long, char, double, char
								(char = string[0]. parser gets only first char)
								
	   /message_processor
	     main.cpp                     execution and main tests
		 manager                      manager of the module
		 core_message_processor       data parsing, class for multithreading
		 speed_synchronisation        Class is created to solving problem with receiving rate
                               		  i.e. module should not process more than X requests in Y seconds.
		 file_handler                 final sorted storage with unique elements for messages and 
		                              saving storage block by block into file (w/o transaction support)
									  (Why "block by block"? - for large files. 
									  I decided not use mapping technologies and simple file rewritting.)
									  
		/input                  folder for input files
		
	## testing:
		data set is described above.
		data types: unsigned long, char, double, char
		main test conditions:
			threads: 1  requests: 1        inSeconds: 1
			threads: 4  requests: 20       inSeconds: 1			
			threads: 8  requests: 2000000  inSeconds: 0.01			
			
	## What else can I change in my design? -
		1. Adding files with data not a list, but checking the presence in the folder
		in accordance with the file name and creation date.
		2. Maybe file mapping, if file is small. 
