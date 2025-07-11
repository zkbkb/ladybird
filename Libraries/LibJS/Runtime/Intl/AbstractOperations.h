/*
 * Copyright (c) 2021-2025, Tim Flynn <trflynn89@ladybird.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/EnumBits.h>
#include <AK/Span.h>
#include <AK/String.h>
#include <AK/Variant.h>
#include <AK/Vector.h>
#include <LibJS/Forward.h>
#include <LibJS/Runtime/Completion.h>
#include <LibJS/Runtime/Date.h>
#include <LibJS/Runtime/Value.h>
#include <LibUnicode/Locale.h>

namespace JS::Intl {

using LocaleKey = Variant<Empty, String>;

struct LocaleOptions {
    Value locale_matcher;
    Optional<LocaleKey> ca; // [[Calendar]]
    Optional<LocaleKey> co; // [[Collation]]
    Optional<LocaleKey> hc; // [[HourCycle]]
    Optional<LocaleKey> kf; // [[CaseFirst]]
    Optional<LocaleKey> kn; // [[Numeric]]
    Optional<LocaleKey> nu; // [[NumberingSystem]]
    Value hour12;
};

struct MatchedLocale {
    String locale;
    Optional<Unicode::Extension> extension;
};

struct ResolvedLocale {
    String locale;
    String icu_locale;
    LocaleKey ca; // [[Calendar]]
    LocaleKey co; // [[Collation]]
    LocaleKey hc; // [[HourCycle]]
    LocaleKey kf; // [[CaseFirst]]
    LocaleKey kn; // [[Numeric]]
    LocaleKey nu; // [[NumberingSystem]]
};

struct ResolvedOptions {
    GC::Ref<Object> options;
    ResolvedLocale resolved_locale;
    LocaleOptions resolution_options;
};

enum class SpecialBehaviors : u8 {
    None = 0,
    RequireOptions = 1 << 1,
    CoerceOptions = 1 << 2,
};
AK_ENUM_BITWISE_OPERATORS(SpecialBehaviors);

using StringOrBoolean = Variant<StringView, bool>;

JS_API bool is_structurally_valid_language_tag(StringView locale);
JS_API String canonicalize_unicode_locale_id(StringView locale);
JS_API bool is_well_formed_currency_code(StringView currency);
JS_API Vector<TimeZoneIdentifier> const& available_named_time_zone_identifiers();
JS_API Optional<TimeZoneIdentifier const&> get_available_named_time_zone_identifier(StringView time_zone_identifier);
JS_API bool is_well_formed_unit_identifier(StringView unit_identifier);
JS_API ThrowCompletionOr<Vector<String>> canonicalize_locale_list(VM&, Value locales);
JS_API Optional<MatchedLocale> lookup_matching_locale_by_prefix(ReadonlySpan<String> requested_locales);
JS_API Optional<MatchedLocale> lookup_matching_locale_by_best_fit(ReadonlySpan<String> requested_locales);
JS_API String insert_unicode_extension_and_canonicalize(Unicode::LocaleID locale_id, Vector<String> attributes, Vector<Unicode::Keyword> keywords);
JS_API ResolvedLocale resolve_locale(ReadonlySpan<String> requested_locales, LocaleOptions const& options, ReadonlySpan<StringView> relevant_extension_keys);
JS_API ThrowCompletionOr<ResolvedOptions> resolve_options(VM& vm, IntlObject& object, Value locales, Value options_value, SpecialBehaviors special_behaviours = SpecialBehaviors::None, Function<void(LocaleOptions&)> modify_resolution_options = {});
JS_API ThrowCompletionOr<GC::Ref<Array>> filter_locales(VM& vm, ReadonlySpan<String> requested_locales, Value options);
JS_API ThrowCompletionOr<GC::Ref<Object>> coerce_options_to_object(VM&, Value options);
JS_API ThrowCompletionOr<StringOrBoolean> get_boolean_or_string_number_format_option(VM& vm, Object const& options, PropertyKey const& property, ReadonlySpan<StringView> string_values, StringOrBoolean fallback);
JS_API ThrowCompletionOr<Optional<int>> default_number_option(VM&, Value value, int minimum, int maximum, Optional<int> fallback);
JS_API ThrowCompletionOr<Optional<int>> get_number_option(VM&, Object const& options, PropertyKey const& property, int minimum, int maximum, Optional<int> fallback);

template<size_t Size>
ThrowCompletionOr<StringOrBoolean> get_boolean_or_string_number_format_option(VM& vm, Object const& options, PropertyKey const& property, StringView const (&string_values)[Size], StringOrBoolean fallback)
{
    return get_boolean_or_string_number_format_option(vm, options, property, ReadonlySpan<StringView> { string_values }, move(fallback));
}

}
