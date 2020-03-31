
; this is a comment


define i32 @bar ()
{
	ret i32 42
}


define void @foo (i32 %arg)
{
	%result = call i32 @bar()

	ret void
}
