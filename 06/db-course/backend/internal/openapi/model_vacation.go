/*
 * balteca
 *
 * API for balteca
 *
 * API version: 1.0.0
 * Generated by: OpenAPI Generator (https://openapi-generator.tech)
 */

package openapi

type Vacation struct {
	Start string `json:"start,omitempty"`

	End string `json:"end,omitempty"`

	Description string `json:"description,omitempty"`

	Id string `json:"id,omitempty"`
}

// AssertVacationRequired checks if the required fields are not zero-ed
func AssertVacationRequired(obj Vacation) error {
	return nil
}

// AssertRecurseVacationRequired recursively checks if required fields are not zero-ed in a nested slice.
// Accepts only nested slice of Vacation (e.g. [][]Vacation), otherwise ErrTypeAssertionError is thrown.
func AssertRecurseVacationRequired(objSlice interface{}) error {
	return AssertRecurseInterfaceRequired(objSlice, func(obj interface{}) error {
		aVacation, ok := obj.(Vacation)
		if !ok {
			return ErrTypeAssertionError
		}
		return AssertVacationRequired(aVacation)
	})
}
