package server

import (
	"net/http"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/contextutils"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/ports"
)

const (
	tokenHeader = "Access-Token"
)

func Middleware(m ports.ProfileManager, next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		ctx := r.Context()

		e, err := m.GetByJWT(ctx, r.Header.Get(tokenHeader))
		if err != nil {
			next.ServeHTTP(w, r)
			return
		}

		ctx = contextutils.SetEmployee(ctx, e)
		r = r.WithContext(ctx)

		next.ServeHTTP(w, r)
	})
}
