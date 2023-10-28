/*
 * balteca
 *
 * API for balteca
 *
 * API version: 1.0.0
 * Generated by: OpenAPI Generator (https://openapi-generator.tech)
 */

package openapi

type GetVacationListResponse struct {
	List []Vacation `json:"list,omitempty"`
}

// AssertGetVacationListResponseRequired checks if the required fields are not zero-ed
func AssertGetVacationListResponseRequired(obj GetVacationListResponse) error {
	for _, el := range obj.List {
		if err := AssertVacationRequired(el); err != nil {
			return err
		}
	}
	return nil
}

// AssertRecurseGetVacationListResponseRequired recursively checks if required fields are not zero-ed in a nested slice.
// Accepts only nested slice of GetVacationListResponse (e.g. [][]GetVacationListResponse), otherwise ErrTypeAssertionError is thrown.
func AssertRecurseGetVacationListResponseRequired(objSlice interface{}) error {
	return AssertRecurseInterfaceRequired(objSlice, func(obj interface{}) error {
		aGetVacationListResponse, ok := obj.(GetVacationListResponse)
		if !ok {
			return ErrTypeAssertionError
		}
		return AssertGetVacationListResponseRequired(aGetVacationListResponse)
	})
}
