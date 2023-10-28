package coder

import (
	"bytes"
	"compress/gzip"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/ports"
	"io"
)

type gzipCoder struct {
	coder ports.Coder
}

var (
	GzipCBOR = gzipCoder{coder: CBOR}
	GzipJSON = gzipCoder{coder: JSON}
)

func (c gzipCoder) Marshal(v any) (string, error) {
	data, err := c.coder.Marshal(v)
	if err != nil {
		return "", err
	}

	var b bytes.Buffer
	w := gzip.NewWriter(&b)
	if _, err := w.Write([]byte(data)); err != nil {
		return "", err
	}
	w.Close()

	return b.String(), err
}

func (c gzipCoder) Unmarshal(data string, v any) error {
	b := bytes.NewBufferString(data)

	r, err := gzip.NewReader(b)
	if err != nil {
		return err
	}
	defer r.Close()

	var bu bytes.Buffer
	if _, err := io.Copy(&bu, r); err != nil {
		return err
	}

	return c.coder.Unmarshal(bu.String(), v)
}
