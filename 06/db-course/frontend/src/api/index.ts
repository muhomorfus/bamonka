import * as openapi from '@/openapi';
import Cookies from 'cookies-ts';

export default new openapi.DefaultApi(new openapi.Configuration({
    basePath: 'http://localhost:9095',
    baseOptions: {
        headers: {
            'Access-Token': new Cookies().get('ACCESS_TOKEN'),
        },
    },
}));