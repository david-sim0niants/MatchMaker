#pragma once

#include "gui/user_property.h"
#include "gui/user_descriptor.h"

namespace matchmaker::gui {

template<typename T>
int compare(const T& lhs, const T& rhs)
{
    if (lhs < rhs)
        return -1;
    else if (lhs > rhs)
        return +1;
    else
        return 0;
}

template<UserProperty property>
int compare(UserDescriptor lhs, UserDescriptor rhs)
{
    switch (property) {
    case UserProperty::Username:
        return compare(lhs.get_username(), rhs.get_username());
    case UserProperty::FirstName:
        return compare(lhs.get_first_name(), rhs.get_first_name());
    case UserProperty::LastName:
        return compare(lhs.get_last_name(), rhs.get_last_name());
    default:
        return 0;
    }
}

inline int compare(UserDescriptor lhs, UserDescriptor rhs, UserProperty criteria)
{
    switch (criteria) {
    case UserProperty::Username:
        return compare<UserProperty::Username>(lhs, rhs);
    case UserProperty::FirstName:
        return compare<UserProperty::FirstName>(lhs, rhs);
    case UserProperty::LastName:
        return compare<UserProperty::LastName>(lhs, rhs);
    default:
        return 0;
    }
}

template<UserProperty property>
struct UserPropertyCompare {
    bool operator()(UserDescriptor lhs, UserDescriptor rhs) const
    {
        return compare<property>(lhs, rhs) < 0;
    }
};

class UserCompare {
public:
    bool operator()(UserDescriptor lhs, UserDescriptor rhs) const
    {
        int c = 0;
        for (int i = 0; i < std::size(properties) && c == 0; ++i)
            c = compare(lhs, rhs, properties[i]);
        return c < 0;
    }

    inline UserProperty get_prioritized_property() const noexcept
    {
        return properties[0];
    }

    void prioritize(UserProperty property)
    {
        UserProperty popped_prop = property;
        for (int i = 0; i < std::size(properties); ++i) {
            std::swap(popped_prop, properties[i]);
            if (popped_prop == property)
                break;
        }
    }

private:
    UserProperty properties[3] {
        UserProperty::Username,
        UserProperty::FirstName,
        UserProperty::LastName
    };
};

}
