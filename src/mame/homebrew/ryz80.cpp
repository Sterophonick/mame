// license:BSD-3-Clause
// copyright-holders:Sterophonick, ry755
/***************************************************************************

    Ry755's Homebrew Z80 Computer
    Driver by Sterophonick

***************************************************************************/

#include "emu.h"
#include "cpu/z80/z80.h"
#include "screen.h"
#include "video/tms9928a.h"

//**************************************************************************
//  Driver Definition
//**************************************************************************

class ryz80_state : public driver_device
{
public:
	ryz80_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this, "maincpu")
		, m_vdp(*this, "vdp")
		, m_screen(*this, "screen")
	{
	}

	void ryz80(machine_config &config);

private:

	virtual void machine_start() override;
	virtual void machine_reset() override;

	void ryz80_map(address_map &map);
	void ryz80_io_map(address_map &map);

	required_device<z80_device> m_maincpu;
	required_device<tms9928a_device> m_vdp;
	required_device<screen_device> m_screen;
};

//**************************************************************************
//  Memory Map
//**************************************************************************

void ryz80_state::ryz80_map(address_map &map)
{
	//EEPROM
	map(0x0000, 0x3FFF).rom(); //BootROM
	map(0x4000, 0x7FFF).rom(); //RomFS

	//SRAM
	map(0x8000, 0x9FFF).ram(); //RY-DOS
	map(0xA000, 0xFDEF).ram(); //User application
	map(0xFDF0, 0xFFFF).ram(); //BIOS Variables + RyFS Buf
}

void ryz80_state::ryz80_io_map(address_map &map)
{
	map(0x10, 0x10).rw("tms9928a", FUNC(tms9928a_device::vram_read), FUNC(tms9928a_device::vram_write)); //VDP_VRAM
	map(0x12, 0x12).rw("tms9928a", FUNC(tms9928a_device::register_read), FUNC(tms9928a_device::register_write)); //VDP_CTRL
}

//**************************************************************************
//  Machine
//**************************************************************************

static INPUT_PORTS_START(ryz80)
INPUT_PORTS_END

void ryz80_state::machine_start()
{
}

void ryz80_state::machine_reset()
{
}

void ryz80_state::ryz80(machine_config &config)
{
	//basic machine hardware
	Z80(config, m_maincpu, 4'000'000); // 4MHz Z80
	m_maincpu->set_addrmap(AS_PROGRAM, &ryz80_state::ryz80_map);
	m_maincpu->set_addrmap(AS_IO, &ryz80_state::ryz80_io_map);

	/* video hardware */
	tms9928a_device &m_vdp(TMS9928A(config, "tms9928a", XTAL(10'738'635)));
	m_vdp.set_screen("screen");
	m_vdp.set_vram_size(0x4000);

	SCREEN(config, "screen", SCREEN_TYPE_RASTER);
}

ROM_START( ryz80 )
	ROM_REGION( 0x0000, "maincpu", 0 )
	ROM_LOAD( "ROM_FINAL.bin",  0x0000, 0x7FFF, CRC(75F2CE3B) SHA1(abc2cddf93d3ced6da848434573f071a0992163a) )
ROM_END

COMP(2020, ryz80, 0, 0, ryz80, ryz80, ryz80_state, empty_init, "ry755", "Ry's Z80 Computer", MACHINE_IS_SKELETON)
