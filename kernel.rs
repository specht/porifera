#[no_std]

static VGA_MEM_START: u32 = 0xb8000;

#[no_split_stack]
#[start]
#[no_mangle]
pub unsafe fn kentry() {
    // 0x1 - blue
    // 0xd - light pink
    //let x: u16 = ('X' as u16) | (0x1d00 as u16);
    //let x: u16 = 'X' as u16;
    *(0xb8650 as *mut u16) = 'X' as u16;
}
