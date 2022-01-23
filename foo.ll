; ModuleID = 'foo.c'
source_filename = "foo.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @foo() #0 {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  %e = alloca i32, align 4
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %f = alloca i32, align 4
  %g = alloca i32, align 4
  %h = alloca i32, align 4
  %i = alloca i32, align 4
  %p = alloca i32*, align 8
  %0 = load i32, i32* %b, align 4
  %1 = load i32, i32* %c, align 4
  %add = add nsw i32 %0, %1
  store i32 %add, i32* %a, align 4
  store i32* %y, i32** %p, align 8
  %2 = load i32, i32* %b, align 4
  %3 = load i32, i32* %c, align 4
  %add1 = add nsw i32 %2, %3
  store i32 %add1, i32* %d, align 4
  %4 = load i32, i32* %a, align 4
  %5 = load i32, i32* %d, align 4
  %add2 = add nsw i32 %4, %5
  %6 = load i32, i32* %x, align 4
  %add3 = add nsw i32 %add2, %6
  %7 = load i32, i32* %y, align 4
  %add4 = add nsw i32 %add3, %7
  store i32 %add4, i32* %f, align 4
  %8 = load i32, i32* %a, align 4
  %9 = load i32, i32* %d, align 4
  %add5 = add nsw i32 %8, %9
  %10 = load i32, i32* %x, align 4
  %add6 = add nsw i32 %add5, %10
  %11 = load i32, i32* %y, align 4
  %add7 = add nsw i32 %add6, %11
  store i32 %add7, i32* %g, align 4
  %12 = load i32, i32* %i, align 4
  %add8 = add nsw i32 %12, 2
  %13 = load i32*, i32** %p, align 8
  store i32 %add8, i32* %13, align 4
  %14 = load i32, i32* %a, align 4
  %15 = load i32, i32* %d, align 4
  %add9 = add nsw i32 %14, %15
  %16 = load i32, i32* %x, align 4
  %add10 = add nsw i32 %add9, %16
  %17 = load i32, i32* %y, align 4
  %add11 = add nsw i32 %add10, %17
  store i32 %add11, i32* %h, align 4
  %18 = load i32, i32* %y, align 4
  %add12 = add nsw i32 %18, 1
  store i32 %add12, i32* %f, align 4
  ret void
}

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 9.0.0 (tags/RELEASE_900/final)"}
