
declare i32 @bar ()


declare void @foo (i32 %arg)


define i32 @main (i32 %argc, i8** %argv)
{
	call void @foo(i32 37)

	%1 = call i32 @bar()
	%2 = add  i32 %1, %argc

	ret i32 %2
}
