namespace olylumoray
{
    class RayTrace :
        C.Cxx.DynamicLibrary
    {
        protected override void
        Init(
            Bam.Core.Module parent)
        {
            base.Init(parent);

            this.CreateHeaderContainer("$(packagedir)/include/olylumoray/*.h");

            var source = this.CreateCxxSourceContainer("$(packagedir)/source/*.cpp");
            source.PrivatePatch(settings =>
            {
                if (settings is C.ICxxOnlyCompilerSettings cxxCompiler)
                {
                    cxxCompiler.ExceptionHandler = C.Cxx.EExceptionHandler.Asynchronous;
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
                else if (settings is GccCommon.ICommonCompilerSettings gccCompiler)
                {
                    gccCompiler.AllWarnings = true;
                    gccCompiler.ExtraWarnings = true;
                    gccCompiler.Pedantic = true;
                    gccCompiler.PositionIndependentCode = true;
                }
            });

            this.PublicPatch((settings, appliedTo) =>
            {
                if (settings is C.ICommonCompilerSettings compiler)
                {
                    compiler.IncludePaths.AddUnique(this.CreateTokenizedString("$(packagedir)/include"));
                }
            });

            this.PrivatePatch(settings =>
            {
                var cxxLinker = settings as C.ICxxOnlyLinkerSettings;
                cxxLinker.StandardLibrary = C.Cxx.EStandardLibrary.libcxx;
            });

            this.CompileAndLinkAgainst<tbb.ThreadBuildingBlocks>(source);
        }
    }
}
