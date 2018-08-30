using Bam.Core;
namespace olylumogui
{
    sealed class olylumoGUI :
        C.Cxx.GUIApplication
    {
        protected override void Init(Module parent)
        {
            base.Init(parent);

            var source = this.CreateCxxSourceContainer("$(packagedir)/source/*.cpp");

            source.PrivatePatch(settings =>
            {
                var cxxCompiler = settings as C.ICxxOnlyCompilerSettings;
                cxxCompiler.LanguageStandard = C.Cxx.ELanguageStandard.Cxx11;
                cxxCompiler.StandardLibrary = C.Cxx.EStandardLibrary.libcxx;
            });

            if (this.BuildEnvironment.Platform.Includes(Bam.Core.EPlatform.OSX))
            {
                this.CompileAndLinkAgainst<Qt.CoreFramework>(source);
            }
            else
            {
                this.CompileAndLinkAgainst<Qt.Core>(source);
            }

            this.PrivatePatch(settings =>
            {
                if (this.Linker is VisualCCommon.LinkerBase)
                {
                    var linker = settings as C.ICommonLinkerSettings;
                    linker.Libraries.Add("shell32.lib"); // for CommandLineToArgvW
                }
            });

            this.ClosingPatch(settings =>
            {
                var vcCompiler = source.Settings as VisualCCommon.ICommonCompilerSettings;
                if (null != vcCompiler)
                {
                    var linker = settings as C.ICommonLinkerSettings;
                    if (vcCompiler.RuntimeLibrary == VisualCCommon.ERuntimeLibrary.MultiThreadedDLL)
                    {
                        linker.Libraries.AddUnique("qtmain.lib");
                    }
                    else if (vcCompiler.RuntimeLibrary == VisualCCommon.ERuntimeLibrary.MultiThreadedDebugDLL)
                    {
                        linker.Libraries.AddUnique("qtmaind.lib");
                    }
                    else
                    {
                        throw new Bam.Core.Exception("Unsupported runtime library, {0}", vcCompiler.RuntimeLibrary.ToString());
                    }
                }
            });
        }
    }
}
