movz w0, #0x3F20, lsl #16
add x0, x0, #4
movz w2, #0x7
orr w2, wzr, w2, lsl #21
mvn w2, w2
and w1, w1, w2
movz w2, #0x1
orr w2, wzr, w2, lsl #21
orr w1, w1, w2
str w1, [x0]

blink_loop:
movz w0, #0x3F20, lsl #16
add x0, x0, #0x1C
movz w1, #0x0002, lsl #16
str w1, [x0]
movz w2, #0x0050, lsl #16

on_delay:
sub w2, w2, #1
cmp w2, #0
b.ne on_delay
movz w0, #0x3F20, lsl #16
add x0, x0, #0x28
movz w1, #0x0002, lsl #16
str w1, [x0]
movz w2, #0x0050, lsl #16

off_delay:
sub w2, w2, #1
cmp w2, #0
b.ne off_delay
b blink_loop