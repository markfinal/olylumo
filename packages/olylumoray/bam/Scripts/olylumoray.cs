using Bam.Core;
namespace olylumoray
{
    class RayTrace :
        C.Cxx.DynamicLibrary
    {
        protected override void
        Init(
            Module parent)
        {
            base.Init(parent);

            this.CreateHeaderContainer("$(packagedir)/include/olylumoray/*.h");

            var source = this.CreateCxxSourceContainer("$(packagedir)/source/*.cpp");
            source.PrivatePatch(settings =>
            {
                if (settings is C.ICxxOnlyCompilerSettings cxxCompiler)
                {
                    cxxCompiler.ExceptionHandler = C.Cxx.EExceptionHandler.Asynchronous;
                    cxxCompiler.EnableRunTimeTypeInfo = false;
                    cxxCompiler.LanguageStandard = C.Cxx.ELanguageStandard.Cxx11;
                    cxxCompiler.StandardLibrary = C.Cxx.EStandardLibrary.libcxx;
                }

                if (settings is VisualCCommon.ICommonCompilerSettings vcCompiler)
                {
                    vcCompiler.WarningLevel = VisualCCommon.EWarningLevel.Level4;
                }
                else if (settings is ClangCommon.ICommonCompilerSettings clangCompiler)
                {
                    clangCompiler.AllWarnings = true;
                    clangCompiler.ExtraWarnings = true;
                    clangCompiler.Pedantic = true;
                }
            });

            this.PublicPatch((settings, appliedTo) =>
            {
                if (settings is C.ICommonCompilerSettings compiler)
                {
                    compiler.IncludePaths.AddUnique(this.CreateTokenizedString("$(packagedir)/include"));
                }
            });
        }
    }
}
