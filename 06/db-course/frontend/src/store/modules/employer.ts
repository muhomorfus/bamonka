import Vuex from 'vuex';
import Cookies from 'cookies-ts';

import * as openapi from '@/openapi';

import API from '@/api';

export default {
    actions: {
        async loadEmployer(ctx: any, nickname: string) {
            try {
                const resp = await API.getEmployeeByNickname(nickname)
                ctx.commit('saveEmployer', resp.data)

                const respBC = await API.getDepartmentBreadcrumbs(resp.data.department.id)
                ctx.commit('saveBreadcrumbs', respBC.data.list)

                const respSub = await API.isSubscribed(resp.data.id)
                ctx.commit('saveSubscription', respSub.data.subscribed)
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        },

        async triggerSubscription(ctx: any) {
            try {
                const resp = await API.subscribe(ctx.state.employer.id)
                ctx.commit('saveSubscription', resp.data.subscribed)
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        },

        async createEmployer(ctx: any) {
            try {
                const e = ctx.state.employer
                const password = ctx.state.employerPassword.password

                await API.createEmployee({
                    name: e.name,
                    nickname: e.nickname,
                    department_id: e.department_id,
                    position: e.position,
                    email: e.email,
                    phone: e.phone,
                    boss_id: e.boss_id,
                    photo: e.photo,
                    level: e.level,
                    teams: e.teams,
                    password: password,
                })

                ctx.dispatch('showError', 'Employer successfully created.')
                window.location.replace('/employee')
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        },

        async addEmptyTeam(ctx: any) {
            ctx.commit('pushEmptyTeam')
        },

        async deleteTeamEntry(ctx: any, index: number) {
            ctx.commit('removeTeamEntry', index)
        },

        async saveEmployer(ctx: any) {
            try {
                await API.updateEmployee(ctx.state.employer.id, ctx.state.employer)

                ctx.dispatch('showError', 'Employer successfully saved.')
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        },
    },

    mutations: {
        saveEmployer(state: any, employer: openapi.Employee) {
            state.employer = employer
        },

        saveSubscription(state: any, subscribed: boolean) {
            state.subscribed = subscribed
        },

        pushEmptyTeam(state: any) {
            if (!state.employer.teams) {
                state.employer.teams = []
            }
            state.employer.teams = [...state.employer.teams, {}]
        },

        removeTeamEntry(state: any, index: number) {
            state.employer.teams.splice(index, 1)
        },
    },

    state: {
        employer: {} as openapi.Employee,
        subscribed: false,
        employerPassword: {
            password: '',
        },
    },

    getters: {
        employer(state: any) {
            return state.employer
        },

        subscribed(state: any) {
            return state.subscribed
        },

        employerPassword(state: any) {
            return state.employerPassword
        },
    },
};