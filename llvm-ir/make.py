#!/usr/bin/env python3


import subprocess

import pathlib

import platform

import argparse

import enum


def llvm_assemble (bitcode_file, llvm_source_file) :

	assert llvm_source_file.suffix == ".ll" and llvm_source_file.exists()
	assert     bitcode_file.suffix == ".bc"

	# LLVM assembler -> https://llvm.org/docs/CommandGuide/llvm-as.html
	cmd = [ "llvm-as", llvm_source_file, "-o", bitcode_file ]

	subprocess.check_call(cmd)

	print(bitcode_file, "<-", llvm_source_file)


def llvm_link (linked_bitcode_file, bitcode_files) :

	for bitcode_file in bitcode_files :
		assert bitcode_file.suffix == ".bc" and bitcode_file.exists()

	assert linked_bitcode_file.suffix == ".bc"

	# LLVM bitcode linker -> https://llvm.org/docs/CommandGuide/llvm-link.html
	cmd = [ "llvm-link", *bitcode_files, "-o", linked_bitcode_file ]

	subprocess.check_call(cmd)

	print(linked_bitcode_file, "<-", "[", *bitcode_files, "]")


def llvm_compile (asm_source_file, llvm_ir_file) :

	assert llvm_ir_file.suffix in [ ".ll", ".bc" ] and llvm_ir_file.exists()
	# llvm_ir_file is either llvm_source_file or bitcode_file

	assert asm_source_file.suffix == ".s"

	# LLVM static compiler -> https://llvm.org/docs/CommandGuide/llc.html
	cmd = [ "llc", llvm_ir_file, "-o", asm_source_file ]

	subprocess.check_call(cmd)

	print(asm_source_file, "<-", llvm_ir_file)


def native_assemble (object_file, asm_source_file) :

	assert asm_source_file.suffix == ".s" and asm_source_file.exists()
	assert     object_file.suffix == ".o"

	# GNU assembler -> https://linux.die.net/man/1/as
	cmd = [ "as", asm_source_file, "-o", object_file ]

	subprocess.check_call(cmd)

	print(object_file, "<-", asm_source_file)


def native_link (native_executable, object_files) :

	for object_file in object_files :
		assert object_file.suffix == ".o" and object_file.exists()

	assert native_executable.suffix == ""

	# GNU linker -> https://linux.die.net/man/1/ld
	cmd = [ "ld", *object_files, "-o", native_executable ]

	if platform.system() == "Darwin" : # macOS
		cmd.append("-lSystem")

	subprocess.check_call(cmd)

	print(native_executable, "<-", "[", *object_files, "]")


def llvm_interpret (bitcode_executable, program_args) :

	assert bitcode_executable.suffix == ".bc" and bitcode_executable.exists()

	# LLVM IR interpreter / JIT compiler -> https://llvm.org/docs/CommandGuide/lli.html
	cmd = [ "lli", bitcode_executable, *program_args ]

	result = subprocess.call(cmd)

	print(bitcode_executable, "->", result)


def native_execute (native_executable, program_args) :

	assert native_executable.suffix == "" and native_executable.exists()

	cmd = [ native_executable, *program_args ]

	result = subprocess.call(cmd)

	print(native_executable, "->", result)


class link_stage (enum.Enum) :


	objectcode = "objectcode"
	bitcode    = "bitcode"


	def __str__ (self) :

		return self.value


class target :


	def __init__ (self, name, sources) :

		self.name    = name
		self.sources = sources


	def build_with_linking_objectcode (self, source_dir, build_dir) :

		object_files = []

		for source in self.sources :

			llvm_source_file = (source_dir / source)
			asm_source_file  = ( build_dir / source).with_suffix(".s")
			llvm_compile(asm_source_file, llvm_source_file)

			object_file      = ( build_dir / source).with_suffix(".o")
			native_assemble(object_file, asm_source_file)

			object_files.append(object_file)

		native_executable    = ( build_dir / self.name)
		native_link(native_executable, object_files)


	def build_with_linking_bitcode (self, source_dir, build_dir) :

		bitcode_files = []

		for source in self.sources :

			llvm_source_file = (source_dir / source)
			bitcode_file     = ( build_dir / source).with_suffix(".bc")
			llvm_assemble(bitcode_file, llvm_source_file)

			bitcode_files.append(bitcode_file)

		bitcode_executable   = ( build_dir / self.name).with_suffix(".bc")
		llvm_link(bitcode_executable, bitcode_files)

		asm_executable       = ( build_dir / self.name).with_suffix(".s")
		llvm_compile(asm_executable, bitcode_executable)

		object_executable    = ( build_dir / self.name).with_suffix(".o")
		native_assemble(object_executable, asm_executable)

		native_executable    = ( build_dir / self.name)
		native_link(native_executable, [ object_executable ])


	def build (self, source_dir, build_dir, link_stage) :

		build_dir.mkdir(parents = True, exist_ok = True)

		if link_stage == link_stage.objectcode :

			self.build_with_linking_objectcode(source_dir, build_dir)

		elif link_stage == link_stage.bitcode :

			self.build_with_linking_bitcode(source_dir, build_dir)

		else :
			assert False


	def run (self, build_dir, program_args) :

		native_executable  = (build_dir / self.name)
		bitcode_executable = (build_dir / self.name).with_suffix(".bc")

		native_execute(native_executable, program_args)

		if bitcode_executable.exists() :
			llvm_interpret(bitcode_executable, program_args)


def parse_args () :

	arg_parser = argparse.ArgumentParser()

	arg_parser.add_argument("--build_dir",
							help     = "path to the build directory",
							type     = pathlib.Path,
							required = True)

	arg_parser.add_argument("--source_dir",
							help     = "path to the source directory",
							type     = pathlib.Path,
							required = True)

	arg_parser.add_argument("--link_stage",
							help     = "link LLVM IR modules or object files",
							type     = link_stage,
							default  = link_stage.objectcode,
							choices  = list(link_stage))

	return arg_parser.parse_args()


def main () :

	args = parse_args()

	app = target(name = "app", sources = [ "main.ll", "foo.ll" ])

	app.build(args.source_dir, args.build_dir, args.link_stage)

	app.run(args.build_dir, program_args = ["these", "are", "program", "args"])


if __name__ == "__main__" :

	main()
