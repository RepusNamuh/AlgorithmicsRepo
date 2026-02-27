include /home/repusnamuh/project/AlgorithmicsRepo/makefile_template

cleanAll: 
	@find . -type d -name .build -exec find {} -type f -delete \;
	@find . -type d -name .debug -exec find {} -type f -delete \;
	@echo "Cleaned up all build and debug directories."