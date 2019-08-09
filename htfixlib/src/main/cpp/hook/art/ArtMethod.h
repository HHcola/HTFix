
#ifndef HTFIX_ARTMETHOD_H
#define HTFIX_ARTMETHOD_H

/**
 * 1. Android O(8.0) Android O的Runtime做了很多优化
 * 2. Android N(7.0/7.1) N 开始采用了混合编译的方式，既有AOT也有JIT.
 * 3. Android M(6.0) 上的ART编译器完全重新实现了：Optimizing
 * 4. Android L(5.0/5.1) 上的ART是在Dalvik上的JIT编译器魔改过来的
 */
namespace HTFix {
    // android 8.0.1,android 8.0
    // http://androidxref.com/8.1.0_r33/xref/art/runtime/asm_support_check.h#20
    class ArtMethod_27_26 {
    protected:
        void * declaring_class_;
        void * access_flags_;
        // Offset to the CodeItem.
         uint32_t dex_code_item_offset_;
        // Index into method_ids of the dex file associated with this method.
        uint32_t dex_method_index_;
        // Entry within a dispatch table for this method. For static/direct methods the index is into
        // the declaringClass.directMethods, for virtual methods the vtable and for interface methods the
        // ifTable.
        uint16_t method_index_;
        // The hotness we measure for this method. Managed by the interpreter. Not atomic, as we allow
        // missing increments: if the method is hot, we will see it eventually.
        uint16_t hotness_count_;
        // Must be the last fields in the method.
        struct PtrSizedFields {
            // Short cuts to declaring_class_->dex_cache_ member for fast compiled code access.
            void* dex_cache_resolved_methods_;

            // Pointer to JNI function registered to this method, or a function to resolve the JNI function,
            // or the profiling data for non-native methods, or an ImtConflictTable, or the
            // single-implementation of an abstract/interface method.
            void* data_;

            // Method dispatch from quick compiled code invokes this pointer which may cause bridging into
            // the interpreter.
            void* entry_point_from_quick_compiled_code_;
        } ptr_sized_fields_;
    };

    // android 7.1 android 7.0
    class ArtMethod_25_24 {
    protected:
        void declaring_class_;

        // Access flags; low 16 bits are defined by spec.
        uint32_t access_flags_;

        /* Dex file fields. The defining dex file is available via declaring_class_->dex_cache_ */

        // Offset to the CodeItem.
        uint32_t dex_code_item_offset_;

        // Index into method_ids of the dex file associated with this method.
        uint32_t dex_method_index_;

        /* End of dex file fields. */

        // Entry within a dispatch table for this method. For static/direct methods the index is into
        // the declaringClass.directMethods, for virtual methods the vtable and for interface methods the
        // ifTable.
        uint16_t method_index_;

        // The hotness we measure for this method. Managed by the interpreter. Not atomic, as we allow
        // missing increments: if the method is hot, we will see it eventually.
        uint16_t hotness_count_;
        // Must be the last fields in the method.
        struct  PtrSizedFields {
            // Short cuts to declaring_class_->dex_cache_ member for fast compiled code access.
            void** dex_cache_resolved_methods_;

            // Short cuts to declaring_class_->dex_cache_ member for fast compiled code access.
            void* dex_cache_resolved_types_;

            // Pointer to JNI function registered to this method, or a function to resolve the JNI function,
            // or the profiling data for non-native methods, or an ImtConflictTable.
            void* entry_point_from_jni_;

            // Method dispatch from quick compiled code invokes this pointer which may cause bridging into
            // the interpreter.
            void* entry_point_from_quick_compiled_code_;
        } ptr_sized_fields_;
    };


    // android 6.0
    class ArtMethod_23 {
    protected:
       void* declaring_class_;

        // Short cuts to declaring_class_->dex_cache_ member for fast compiled code access.
        void* dex_cache_resolved_methods_;

        // Short cuts to declaring_class_->dex_cache_ member for fast compiled code access.
        void* dex_cache_resolved_types_;

        // Access flags; low 16 bits are defined by spec.
        uint32_t access_flags_;

        /* Dex file fields. The defining dex file is available via declaring_class_->dex_cache_ */

        // Offset to the CodeItem.
        uint32_t dex_code_item_offset_;

        // Index into method_ids of the dex file associated with this method.
        uint32_t dex_method_index_;

        /* End of dex file fields. */

        // Entry within a dispatch table for this method. For static/direct methods the index is into
        // the declaringClass.directMethods, for virtual methods the vtable and for interface methods the
        // ifTable.
        uint32_t method_index_;
        // Must be the last fields in the method.
        // PACKED(4) is necessary for the correctness of
        // RoundUp(OFFSETOF_MEMBER(ArtMethod, ptr_sized_fields_), pointer_size).
        struct  PtrSizedFields {
            // Method dispatch from the interpreter invokes this pointer which may cause a bridge into
            // compiled code.
            void* entry_point_from_interpreter_;

            // Pointer to JNI function registered to this method, or a function to resolve the JNI function.
            void* entry_point_from_jni_;

            // Method dispatch from quick compiled code invokes this pointer which may cause bridging into
            // the interpreter.
            void* entry_point_from_quick_compiled_code_;
        } ptr_sized_fields_;
    };

    // android 5.1
    // http://androidxref.com/5.1.1_r6/xref/art/runtime/mirror/art_method.h
    class ArtMethod_22 {
    protected:
        // Field order required by test "ValidateFieldOrderOfJavaCppUnionClasses".
        // The class we are a part of.
        void * declaring_class_;

        // Short cuts to declaring_class_->dex_cache_ member for fast compiled code access.
        void * dex_cache_resolved_methods_;

        // Short cuts to declaring_class_->dex_cache_ member for fast compiled code access.
        void * dex_cache_resolved_types_;

        // Access flags; low 16 bits are defined by spec.
        uint32_t access_flags_;

        /* Dex file fields. The defining dex file is available via declaring_class_->dex_cache_ */

        // Offset to the CodeItem.
        uint32_t dex_code_item_offset_;

        // Index into method_ids of the dex file associated with this method.
        uint32_t dex_method_index_;

        /* End of dex file fields. */

        // Entry within a dispatch table for this method. For static/direct methods the index is into
        // the declaringClass.directMethods, for virtual methods the vtable and for interface methods the
        // ifTable.
        uint32_t method_index_;

        // Fake padding field gets inserted here.

        // Must be the last fields in the method.
        struct  PtrSizedFields {
            // Method dispatch from the interpreter invokes this pointer which may cause a bridge into
            // compiled code.
            void* entry_point_from_interpreter_;

            // Pointer to JNI function registered to this method, or a function to resolve the JNI function.
            void* entry_point_from_jni_;

            // Method dispatch from quick compiled code invokes this pointer which may cause bridging into
            // portable compiled code or the interpreter.
            void* entry_point_from_quick_compiled_code_;

            // Method dispatch from portable compiled code invokes this pointer which may cause bridging
            // into quick compiled code or the interpreter. Last to simplify entrypoint logic.
#if defined(ART_USE_PORTABLE_COMPILER)
            void* entry_point_from_portable_compiled_code_;
#endif
        } ptr_sized_fields_;

        static void * java_lang_reflect_ArtMethod_;
    };

    // android 5.0
    class ArtMethod_21 {
    protected:
        // Field order required by test "ValidateFieldOrderOfJavaCppUnionClasses".
        // The class we are a part of.
        void* declaring_class_;

        // Short cuts to declaring_class_->dex_cache_ member for fast compiled code access.
        void* dex_cache_resolved_methods_;

        // Short cuts to declaring_class_->dex_cache_ member for fast compiled code access.
        void* dex_cache_resolved_types_;

        // Short cuts to declaring_class_->dex_cache_ member for fast compiled code access.
        void* dex_cache_strings_;

        // Method dispatch from the interpreter invokes this pointer which may cause a bridge into
        // compiled code.
        uint64_t entry_point_from_interpreter_;

        // Pointer to JNI function registered to this method, or a function to resolve the JNI function.
        uint64_t entry_point_from_jni_;

        // Method dispatch from portable compiled code invokes this pointer which may cause bridging into
        // quick compiled code or the interpreter.
#if defined(ART_USE_PORTABLE_COMPILER)
        uint64_t entry_point_from_portable_compiled_code_;
#endif

        // Method dispatch from quick compiled code invokes this pointer which may cause bridging into
        // portable compiled code or the interpreter.
        uint64_t entry_point_from_quick_compiled_code_;

        // Pointer to a data structure created by the compiler and used by the garbage collector to
        // determine which registers hold live references to objects within the heap. Keyed by native PC
        // offsets for the quick compiler and dex PCs for the portable.
        uint64_t gc_map_;

        // Access flags; low 16 bits are defined by spec.
        uint32_t access_flags_;

        /* Dex file fields. The defining dex file is available via declaring_class_->dex_cache_ */

        // Offset to the CodeItem.
        uint32_t dex_code_item_offset_;

        // Index into method_ids of the dex file associated with this method.
        uint32_t dex_method_index_;

        /* End of dex file fields. */

        // Entry within a dispatch table for this method. For static/direct methods the index is into
        // the declaringClass.directMethods, for virtual methods the vtable and for interface methods the
        // ifTable.
        uint32_t method_index_;

        static void* java_lang_reflect_ArtMethod_;
    };


    // android 4.4w android 4.4
    class ArtMethod_20_19 {
    protected:
        // Field order required by test "ValidateFieldOrderOfJavaCppUnionClasses".
        // The class we are a part of
        void* declaring_class_;

        // short cuts to declaring_class_->dex_cache_ member for fast compiled code access
        void* dex_cache_initialized_static_storage_;

        // short cuts to declaring_class_->dex_cache_ member for fast compiled code access
        void* dex_cache_resolved_methods_;

        // short cuts to declaring_class_->dex_cache_ member for fast compiled code access
        void* dex_cache_resolved_types_;

        // short cuts to declaring_class_->dex_cache_ member for fast compiled code access
        void* dex_cache_strings_;

        // Access flags; low 16 bits are defined by spec.
        uint32_t access_flags_;

        // Offset to the CodeItem.
        uint32_t code_item_offset_;

        // Architecture-dependent register spill mask
        uint32_t core_spill_mask_;

        // Compiled code associated with this method for callers from managed code.
        // May be compiled managed code or a bridge for invoking a native method.
        // TODO: Break apart this into portable and quick.
        const void* entry_point_from_compiled_code_;

        // Called by the interpreter to execute this method.
        void* entry_point_from_interpreter_;

        // Architecture-dependent register spill mask
        uint32_t fp_spill_mask_;

        // Total size in bytes of the frame
        size_t frame_size_in_bytes_;

        // Garbage collection map of native PC offsets (quick) or dex PCs (portable) to reference bitmaps.
        const uint8_t* gc_map_;

        // Mapping from native pc to dex pc
        const uint32_t* mapping_table_;

        // Index into method_ids of the dex file associated with this method
        uint32_t method_dex_index_;

        // For concrete virtual methods, this is the offset of the method in Class::vtable_.
        //
        // For abstract methods in an interface class, this is the offset of the method in
        // "iftable_->Get(n)->GetMethodArray()".
        //
        // For static and direct methods this is the index in the direct methods table.
        uint32_t method_index_;

        // The target native method registered with this method
        const void* native_method_;

        // When a register is promoted into a register, the spill mask holds which registers hold dex
        // registers. The first promoted register's corresponding dex register is vmap_table_[1], the Nth
        // is vmap_table_[N]. vmap_table_[0] holds the length of the table.
        const uint16_t* vmap_table_;

        static void* java_lang_reflect_ArtMethod_;
    };

}
#endif //HTFIX_ARTMETHOD_H
