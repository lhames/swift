# swift_build_support/products/clang.py -------------------------*- python -*-
#
# This source file is part of the Swift.org open source project
#
# Copyright (c) 2014 - 2017 Apple Inc. and the Swift project authors
# Licensed under Apache License v2.0 with Runtime Library Exception
#
# See https://swift.org/LICENSE.txt for license information
# See https://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
#
# ----------------------------------------------------------------------------

from . import product


class LLBuild(product.Product):
    @classmethod
    def is_build_script_impl_product(cls):
        """is_build_script_impl_product -> bool

        Whether this product is produced by build-script-impl.
        """
        return True

    @classmethod
    def get_dependencies(cls):
        return [product.CMark,
                product.LLVM,
                product.LibCXX,
                product.LibICU,
                product.Swift,
                product.LLDB,
                product.LibDispatch,
                product.Foundation,
                product.XCTest]
