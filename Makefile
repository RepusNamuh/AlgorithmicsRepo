include /home/repusnamuh/project/AlgorithmicsRepo/makefile_template

cleanAll: 
	@find . -type d -name build -exec find {} -type f -delete \;