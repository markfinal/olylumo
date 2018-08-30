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
        }
    }
}
