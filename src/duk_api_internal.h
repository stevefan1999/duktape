/*
 *  Internal API calls which have (stack and other) semantics similar
 *  to the public API.
 */

#ifndef DUK_API_INTERNAL_H_INCLUDED
#define DUK_API_INTERNAL_H_INCLUDED

/* duk_push_sprintf constants */
#define DUK_PUSH_SPRINTF_INITIAL_SIZE  256L
#define DUK_PUSH_SPRINTF_SANITY_LIMIT  (1L * 1024L * 1024L * 1024L)

/* Flag ORed to err_code to indicate __FILE__ / __LINE__ is not
 * blamed as source of error for error fileName / lineNumber.
 */
#define DUK_ERRCODE_FLAG_NOBLAME_FILELINE  (1L << 24)

/* Valstack resize flags */
#define DUK_VSRESIZE_FLAG_SHRINK           (1 << 0)
#define DUK_VSRESIZE_FLAG_COMPACT          (1 << 1)
#define DUK_VSRESIZE_FLAG_THROW            (1 << 2)

/* Current convention is to use duk_size_t for value stack sizes and global indices,
 * and duk_idx_t for local frame indices.
 */
DUK_INTERNAL_DECL
duk_bool_t duk_valstack_resize_raw(duk_context *ctx,
                                   duk_size_t min_new_size,
                                   duk_small_uint_t flags);

#if defined(DUK_USE_VERBOSE_ERRORS) && defined(DUK_USE_PARANOID_ERRORS)
DUK_INTERNAL_DECL const char *duk_get_type_name(duk_context *ctx, duk_idx_t idx);
#endif

DUK_INTERNAL_DECL duk_tval *duk_get_tval(duk_context *ctx, duk_idx_t idx);
DUK_INTERNAL_DECL duk_tval *duk_require_tval(duk_context *ctx, duk_idx_t idx);
DUK_INTERNAL_DECL void duk_push_tval(duk_context *ctx, duk_tval *tv);

/* Push the current 'this' binding; throw TypeError if binding is not object
 * coercible (CheckObjectCoercible).
 */
DUK_INTERNAL_DECL void duk_push_this_check_object_coercible(duk_context *ctx);

/* duk_push_this() + CheckObjectCoercible() + duk_to_object() */
DUK_INTERNAL_DECL duk_hobject *duk_push_this_coercible_to_object(duk_context *ctx);

/* duk_push_this() + CheckObjectCoercible() + duk_to_string() */
DUK_INTERNAL_DECL duk_hstring *duk_push_this_coercible_to_string(duk_context *ctx);

/* Get a borrowed duk_tval pointer to the current 'this' binding.  Caller must
 * make sure there's an active callstack entry.  Note that the returned pointer
 * is unstable with regards to side effects.
 */
DUK_INTERNAL_DECL duk_tval *duk_get_borrowed_this_tval(duk_context *ctx);

/* XXX: add fastint support? */
#define duk_push_u64(ctx,val) \
	duk_push_number((ctx), (duk_double_t) (val))
#define duk_push_i64(ctx,val) \
	duk_push_number((ctx), (duk_double_t) (val))

/* duk_push_(u)int() is guaranteed to support at least (un)signed 32-bit range */
#define duk_push_u32(ctx,val) \
	duk_push_uint((ctx), (duk_uint_t) (val))
#define duk_push_i32(ctx,val) \
	duk_push_int((ctx), (duk_int_t) (val))

/* sometimes stack and array indices need to go on the stack */
#define duk_push_idx(ctx,val) \
	duk_push_int((ctx), (duk_int_t) (val))
#define duk_push_uarridx(ctx,val) \
	duk_push_uint((ctx), (duk_uint_t) (val))
#define duk_push_size_t(ctx,val) \
	duk_push_uint((ctx), (duk_uint_t) (val))  /* XXX: assumed to fit for now */

DUK_INTERNAL_DECL duk_hstring *duk_get_hstring(duk_context *ctx, duk_idx_t idx);
DUK_INTERNAL_DECL duk_hobject *duk_get_hobject(duk_context *ctx, duk_idx_t idx);
DUK_INTERNAL_DECL duk_hbuffer *duk_get_hbuffer(duk_context *ctx, duk_idx_t idx);
DUK_INTERNAL_DECL duk_hthread *duk_get_hthread(duk_context *ctx, duk_idx_t idx);
DUK_INTERNAL_DECL duk_hcompfunc *duk_get_hcompfunc(duk_context *ctx, duk_idx_t idx);
DUK_INTERNAL_DECL duk_hnatfunc *duk_get_hnatfunc(duk_context *ctx, duk_idx_t idx);

DUK_INTERNAL_DECL duk_hobject *duk_get_hobject_with_class(duk_context *ctx, duk_idx_t idx, duk_small_uint_t classnum);

#if 0  /* This would be pointless: unexpected type and lightfunc would both return NULL */
DUK_INTERNAL_DECL duk_hobject *duk_get_hobject_or_lfunc(duk_context *ctx, duk_idx_t idx);
#endif
DUK_INTERNAL_DECL duk_hobject *duk_get_hobject_or_lfunc_coerce(duk_context *ctx, duk_idx_t idx);

#if 0  /*unused*/
DUK_INTERNAL_DECL void *duk_get_voidptr(duk_context *ctx, duk_idx_t idx);
#endif

DUK_INTERNAL_DECL duk_hstring *duk_to_hstring(duk_context *ctx, duk_idx_t idx);
#if defined(DUK_USE_DEBUGGER_SUPPORT)  /* only needed by debugger for now */
DUK_INTERNAL_DECL duk_hstring *duk_safe_to_hstring(duk_context *ctx, duk_idx_t idx);
#endif
DUK_INTERNAL_DECL void duk_to_object_class_string_top(duk_context *ctx);
DUK_INTERNAL_DECL void duk_push_hobject_class_string(duk_context *ctx, duk_hobject *h);

DUK_INTERNAL_DECL duk_int_t duk_to_int_clamped_raw(duk_context *ctx, duk_idx_t idx, duk_int_t minval, duk_int_t maxval, duk_bool_t *out_clamped);  /* out_clamped=NULL, RangeError if outside range */
DUK_INTERNAL_DECL duk_int_t duk_to_int_clamped(duk_context *ctx, duk_idx_t idx, duk_int_t minval, duk_int_t maxval);
DUK_INTERNAL_DECL duk_int_t duk_to_int_check_range(duk_context *ctx, duk_idx_t idx, duk_int_t minval, duk_int_t maxval);
#if defined(DUK_USE_BUFFEROBJECT_SUPPORT)
DUK_INTERNAL_DECL duk_uint8_t duk_to_uint8clamped(duk_context *ctx, duk_idx_t idx);
#endif

DUK_INTERNAL_DECL duk_hstring *duk_require_hstring(duk_context *ctx, duk_idx_t idx);
DUK_INTERNAL_DECL duk_hobject *duk_require_hobject(duk_context *ctx, duk_idx_t idx);
DUK_INTERNAL_DECL duk_hbuffer *duk_require_hbuffer(duk_context *ctx, duk_idx_t idx);
DUK_INTERNAL_DECL duk_hthread *duk_require_hthread(duk_context *ctx, duk_idx_t idx);
DUK_INTERNAL_DECL duk_hcompfunc *duk_require_hcompfunc(duk_context *ctx, duk_idx_t idx);
DUK_INTERNAL_DECL duk_hnatfunc *duk_require_hnatfunc(duk_context *ctx, duk_idx_t idx);

DUK_INTERNAL_DECL duk_hobject *duk_require_hobject_with_class(duk_context *ctx, duk_idx_t idx, duk_small_uint_t classnum);

DUK_INTERNAL_DECL duk_hobject *duk_require_hobject_or_lfunc(duk_context *ctx, duk_idx_t idx);
DUK_INTERNAL_DECL duk_hobject *duk_require_hobject_or_lfunc_coerce(duk_context *ctx, duk_idx_t idx);

DUK_INTERNAL_DECL void duk_push_hstring(duk_context *ctx, duk_hstring *h);
DUK_INTERNAL_DECL void duk_push_hstring_stridx(duk_context *ctx, duk_small_int_t stridx);
DUK_INTERNAL_DECL void duk_push_hobject(duk_context *ctx, duk_hobject *h);
DUK_INTERNAL_DECL void duk_push_hbuffer(duk_context *ctx, duk_hbuffer *h);
#define duk_push_hthread(ctx,h) \
	duk_push_hobject((ctx), (duk_hobject *) (h))
#define duk_push_hcompfunc(ctx,h) \
	duk_push_hobject((ctx), (duk_hobject *) (h))
#define duk_push_hnatfunc(ctx,h) \
	duk_push_hobject((ctx), (duk_hobject *) (h))
DUK_INTERNAL_DECL void duk_push_hobject_bidx(duk_context *ctx, duk_small_int_t builtin_idx);
DUK_INTERNAL_DECL duk_idx_t duk_push_object_helper(duk_context *ctx, duk_uint_t hobject_flags_and_class, duk_small_int_t prototype_bidx);
DUK_INTERNAL_DECL duk_idx_t duk_push_object_helper_proto(duk_context *ctx, duk_uint_t hobject_flags_and_class, duk_hobject *proto);
DUK_INTERNAL_DECL duk_idx_t duk_push_object_internal(duk_context *ctx);
DUK_INTERNAL_DECL duk_idx_t duk_push_compiledfunction(duk_context *ctx);
DUK_INTERNAL_DECL void duk_push_c_function_noexotic(duk_context *ctx, duk_c_function func, duk_int_t nargs);
DUK_INTERNAL_DECL void duk_push_c_function_noconstruct_noexotic(duk_context *ctx, duk_c_function func, duk_int_t nargs);

DUK_INTERNAL_DECL void duk_push_string_funcptr(duk_context *ctx, duk_uint8_t *ptr, duk_size_t sz);
DUK_INTERNAL_DECL void duk_push_lightfunc_name(duk_context *ctx, duk_tval *tv);
DUK_INTERNAL_DECL void duk_push_lightfunc_tostring(duk_context *ctx, duk_tval *tv);
DUK_INTERNAL_DECL duk_hbufobj *duk_push_bufobj_raw(duk_context *ctx, duk_uint_t hobject_flags_and_class, duk_small_int_t prototype_bidx);

DUK_INTERNAL_DECL const char *duk_push_string_readable(duk_context *ctx, duk_idx_t idx);
DUK_INTERNAL_DECL const char *duk_push_string_tval_readable(duk_context *ctx, duk_tval *tv);
DUK_INTERNAL_DECL const char *duk_push_string_tval_readable_error(duk_context *ctx, duk_tval *tv);

DUK_INTERNAL_DECL duk_bool_t duk_get_prop_stridx(duk_context *ctx, duk_idx_t obj_idx, duk_small_int_t stridx);     /* [] -> [val] */
DUK_INTERNAL_DECL duk_bool_t duk_put_prop_stridx(duk_context *ctx, duk_idx_t obj_idx, duk_small_int_t stridx);     /* [val] -> [] */
DUK_INTERNAL_DECL duk_bool_t duk_del_prop_stridx(duk_context *ctx, duk_idx_t obj_idx, duk_small_int_t stridx);     /* [] -> [] */
DUK_INTERNAL_DECL duk_bool_t duk_has_prop_stridx(duk_context *ctx, duk_idx_t obj_idx, duk_small_int_t stridx);     /* [] -> [] */

DUK_INTERNAL_DECL duk_bool_t duk_get_prop_stridx_boolean(duk_context *ctx, duk_idx_t obj_idx, duk_small_int_t stridx, duk_bool_t *out_has_prop);  /* [] -> [] */

DUK_INTERNAL_DECL void duk_xdef_prop(duk_context *ctx, duk_idx_t obj_idx, duk_small_uint_t desc_flags);  /* [key val] -> [] */
DUK_INTERNAL_DECL void duk_xdef_prop_index(duk_context *ctx, duk_idx_t obj_idx, duk_uarridx_t arr_idx, duk_small_uint_t desc_flags);  /* [val] -> [] */
DUK_INTERNAL_DECL void duk_xdef_prop_stridx(duk_context *ctx, duk_idx_t obj_idx, duk_small_int_t stridx, duk_small_uint_t desc_flags);  /* [val] -> [] */
DUK_INTERNAL_DECL void duk_xdef_prop_stridx_builtin(duk_context *ctx, duk_idx_t obj_idx, duk_small_int_t stridx, duk_small_int_t builtin_idx, duk_small_uint_t desc_flags);  /* [] -> [] */
DUK_INTERNAL_DECL void duk_xdef_prop_stridx_thrower(duk_context *ctx, duk_idx_t obj_idx, duk_small_int_t stridx, duk_small_uint_t desc_flags);  /* [] -> [] */

/* These are macros for now, but could be separate functions to reduce code
 * footprint (check call site count before refactoring).
 */
#define duk_xdef_prop_wec(ctx,obj_idx) \
	duk_xdef_prop((ctx), (obj_idx), DUK_PROPDESC_FLAGS_WEC)
#define duk_xdef_prop_index_wec(ctx,obj_idx,arr_idx) \
	duk_xdef_prop_index((ctx), (obj_idx), (arr_idx), DUK_PROPDESC_FLAGS_WEC)
#define duk_xdef_prop_stridx_wec(ctx,obj_idx,stridx) \
	duk_xdef_prop_stridx((ctx), (obj_idx), (stridx), DUK_PROPDESC_FLAGS_WEC)

/* Set object 'length'. */
DUK_INTERNAL_DECL void duk_set_length(duk_context *ctx, duk_idx_t idx, duk_size_t length);

/* Raw internal valstack access macros: access is unsafe so call site
 * must have a guarantee that the index is valid.  When that is the case,
 * using these macro results in faster and smaller code than duk_get_tval().
 * Both 'ctx' and 'idx' are evaluted multiple times, but only for asserts.
 */
#define DUK_ASSERT_VALID_NEGIDX(ctx,idx) \
	(DUK_ASSERT_EXPR((idx) < 0), DUK_ASSERT_EXPR(duk_is_valid_index((ctx), (idx))))
#define DUK_ASSERT_VALID_POSIDX(ctx,idx) \
	(DUK_ASSERT_EXPR((idx) >= 0), DUK_ASSERT_EXPR(duk_is_valid_index((ctx), (idx))))
#define DUK_GET_TVAL_NEGIDX(ctx,idx) \
	(DUK_ASSERT_VALID_NEGIDX((ctx),(idx)), ((duk_hthread *) (ctx))->valstack_top + (idx))
#define DUK_GET_TVAL_POSIDX(ctx,idx) \
	(DUK_ASSERT_VALID_POSIDX((ctx),(idx)), ((duk_hthread *) (ctx))->valstack_bottom + (idx))
#define DUK_GET_HOBJECT_NEGIDX(ctx,idx) \
	(DUK_ASSERT_VALID_NEGIDX((ctx),(idx)), DUK_TVAL_GET_OBJECT(((duk_hthread *) (ctx))->valstack_top + (idx)))
#define DUK_GET_HOBJECT_POSIDX(ctx,idx) \
	(DUK_ASSERT_VALID_POSIDX((ctx),(idx)), DUK_TVAL_GET_OBJECT(((duk_hthread *) (ctx))->valstack_bottom + (idx)))

#endif  /* DUK_API_INTERNAL_H_INCLUDED */
