import Vuex from 'vuex';
import Cookies from 'cookies-ts';

import * as openapi from '@/openapi';

import API from '@/api';

export default {
    actions: {
        async loadTeam(ctx: any, id: string) {
            try {
                const resp = await API.getTeam(id)
                ctx.commit('saveTeam', resp.data)
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        },

        async saveTeam(ctx: any) {
            try {
                await API.updateTeam(ctx.state.team.id, ctx.state.team)
                ctx.dispatch('showError', 'Team successfully saved.')
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        },

        async createTeam(ctx: any) {
            try {
                await API.createTeam({
                    name: ctx.state.team.name,
                    description: ctx.state.team.description,
                })
                ctx.dispatch('showError', 'Team successfully created.')
                window.location.replace('/teams')
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        },

        async loadTeamMembers(ctx: any, id: string) {
            try {
                const resp = await API.getEmployeeByTeam(id)
                ctx.commit('saveTeamMembers', resp.data.list)
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        }
    },

    mutations: {
        saveTeam(state: any, team: openapi.Team) {
            state.team = team
        },

        saveTeamMembers(state: any, emp: Array<openapi.Employee>) {
            state.teamMembers = emp
        }
    },

    state: {
        team: {} as openapi.Team,
        teamMembers: Array<openapi.Employee>(),
    },

    getters: {
        team(state: any) {
            return state.team
        },

        teamMembers(state: any) {
            return state.teamMembers
        },
    },
};