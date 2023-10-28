/*
 * balteca
 *
 * API for balteca
 *
 * API version: 1.0.0
 * Generated by: OpenAPI Generator (https://openapi-generator.tech)
 */

package openapi

type DepartmentAllOf struct {
	Description string `json:"description,omitempty"`

	ParentId *string `json:"parent_id,omitempty"`
}

// AssertDepartmentAllOfRequired checks if the required fields are not zero-ed
func AssertDepartmentAllOfRequired(obj DepartmentAllOf) error {
	return nil
}

// AssertRecurseDepartmentAllOfRequired recursively checks if required fields are not zero-ed in a nested slice.
// Accepts only nested slice of DepartmentAllOf (e.g. [][]DepartmentAllOf), otherwise ErrTypeAssertionError is thrown.
func AssertRecurseDepartmentAllOfRequired(objSlice interface{}) error {
	return AssertRecurseInterfaceRequired(objSlice, func(obj interface{}) error {
		aDepartmentAllOf, ok := obj.(DepartmentAllOf)
		if !ok {
			return ErrTypeAssertionError
		}
		return AssertDepartmentAllOfRequired(aDepartmentAllOf)
	})
}
