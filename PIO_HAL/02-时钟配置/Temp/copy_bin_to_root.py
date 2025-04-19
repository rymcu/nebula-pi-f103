Import("env")
import shutil
import os

def copy_bin_after_build(source, target, env):
    try:
        # 获取生成的 .bin 文件真实路径（核心代码）
        build_dir = env.subst("$BUILD_DIR")
        prog_name = env.subst("$PROGNAME")
        bin_source = os.path.join(build_dir, f"{prog_name}.bin")
        
        # 调试输出路径信息
        print(f"Debug -> 原始路径: {bin_source}")
        if not os.path.exists(bin_source):
            raise FileNotFoundError(f".bin 文件未生成: {bin_source}")

        # 复制到项目根目录
        bin_dest = os.path.join(env["PROJECT_DIR"], os.path.basename(bin_source))
        shutil.copy(bin_source, bin_dest)
        print(f"✅ 成功复制到根目录: {os.path.basename(bin_dest)}")

    except Exception as e:
        print(f"❌ 错误: {str(e)}")

# 关键：绑定到构建完成后的动作（而非中间步骤）
env.AddPostAction("buildprog", copy_bin_after_build)