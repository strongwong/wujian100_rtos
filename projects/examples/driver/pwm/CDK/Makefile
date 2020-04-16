.PHONY: clean All

All:
	@echo "----------Building project:[ wujian100_open-pwm - BuildSet ]----------"
	@
	@make -f "wujian100_open-pwm.mk" MakeIntermediateDirs && make -f "wujian100_open-pwm.mk" -j 12
	@echo Executing Post Build commands ...
	@export CDKPath="D:/MyProgram/C-Sky/CDK" ProjectPath="E:/work/T_head/wujian100_open-master/sdk/projects/examples/driver/pwm/CDK" && "E:/work/T_head/wujian100_open-master/sdk/projects/examples/driver/pwm/CDK//../../../../../utilities//aft_build.sh"
	@echo Done

clean:
	@echo "----------Cleaning project:[ wujian100_open-pwm - BuildSet ]----------"
	@make -f  "wujian100_open-pwm.mk" clean
