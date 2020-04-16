.PHONY: clean All

All:
	@echo "----------Building project:[ wujian100_open-vic - BuildSet ]----------"
	@
	@make -f "wujian100_open-vic.mk" MakeIntermediateDirs && make -f "wujian100_open-vic.mk" -j 12
	@echo Executing Post Build commands ...
	@export CDKPath="D:/MyProgram/C-Sky/CDK" ProjectPath="E:/work/T_head/test/projects/examples/core/vic/CDK" && "E:/work/T_head/test/projects/examples/core/vic/CDK//../../../../../utilities//aft_build.sh"
	@echo Done

clean:
	@echo "----------Cleaning project:[ wujian100_open-vic - BuildSet ]----------"
	@make -f  "wujian100_open-vic.mk" clean
