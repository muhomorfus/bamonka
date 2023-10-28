/*
 * balteca
 *
 * API for balteca
 *
 * API version: 1.0.0
 * Generated by: OpenAPI Generator (https://openapi-generator.tech)
 */

package openapi

type GetEmployeeListResponse struct {
	List []Employee `json:"list,omitempty"`
}

// AssertGetEmployeeListResponseRequired checks if the required fields are not zero-ed
func AssertGetEmployeeListResponseRequired(obj GetEmployeeListResponse) error {
	for _, el := range obj.List {
		if err := AssertEmployeeRequired(el); err != nil {
			return err
		}
	}
	return nil
}

// AssertRecurseGetEmployeeListResponseRequired recursively checks if required fields are not zero-ed in a nested slice.
// Accepts only nested slice of GetEmployeeListResponse (e.g. [][]GetEmployeeListResponse), otherwise ErrTypeAssertionError is thrown.
func AssertRecurseGetEmployeeListResponseRequired(objSlice interface{}) error {
	return AssertRecurseInterfaceRequired(objSlice, func(obj interface{}) error {
		aGetEmployeeListResponse, ok := obj.(GetEmployeeListResponse)
		if !ok {
			return ErrTypeAssertionError
		}
		return AssertGetEmployeeListResponseRequired(aGetEmployeeListResponse)
	})
}
