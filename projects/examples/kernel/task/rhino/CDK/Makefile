.PHONY: clean All

All:
	@echo "----------Building project:[ wujian100_open-rhino-task - BuildSet ]----------"
	@
	@make -f "wujian100_open-rhino-task.mk" MakeIntermediateDirs && make -f "wujian100_open-rhino-task.mk" -j 12
	@echo Executing Post Build commands ...
	@export CDKPath="D:/MyProgram/C-Sky/CDK" ProjectPath="E:/work/T_head/test/projects/examples/kernel/task/rhino/CDK" && "E:/work/T_head/test/projects/examples/kernel/task/rhino/CDK//../../../../../../utilities//aft_build.sh"
	@echo Done

clean:
	@echo "----------Cleaning project:[ wujian100_open-rhino-task - BuildSet ]----------"
	@make -f  "wujian100_open-rhino-task.mk" clean
