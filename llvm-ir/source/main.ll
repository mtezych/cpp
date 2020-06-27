
;
; 2019 EuroLLVM: Vince Bridgers & Felipe de Azevedo Piovezan
; LLVM IR Tutorial - Phis, GEPs and other things, oh my!
;
;  ~ https://www.youtube.com/watch?v=m8G_S5LwlTo
;

;
; LLVM IR Language Reference Manual
;
; ~ https://llvm.org/docs/LangRef.html
;


declare i32 @bar ()


declare void @foo (i32 %arg)


declare i32 @puts (i8* %str)


@str = constant [8 x i8] c"LLVM-IR\00"


define i32 @main (i32 %argc, i8** %argv)
{
	call void @foo(i32 37)

	%1 = call i32 @bar()
	%2 = add  i32 %1, %argc

	%3 = getelementptr [8 x i8], [8 x i8]* @str, i32 0, i32 0
	%4 = call i32 @puts(i8* %3)

	ret i32 %2
}
