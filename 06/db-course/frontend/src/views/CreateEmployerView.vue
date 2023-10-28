<template>
  <main class="container">
      <br>

    <div class="row">
      <div class="col-md-2"></div>
      <div class="col-md-8">

        <div class="card mb-3">
          <div class="card-body">
            <h1>Creating user</h1>

            <form @submit.prevent="createEmployer">
              <div class="mb-3">
                <label for="name" class="form-label">Name</label>
                <input required type="text" class="form-control" id="name"
                       placeholder="Stas Baluev" v-model="employer.name">
              </div>

              <div class="mb-3">
                <label for="nickname" class="form-label">Nickname</label>
                <input required type="text" class="form-control" id="nickname"
                       placeholder="sultanov" v-model="employer.nickname">
              </div>

              <div class="mb-3">
                <label for="department" class="form-label">Department</label>
                <select required class="form-select" id="department" aria-label="Select parent department" v-model="employer.department_id">
                  <option v-for="d in departments" :key="d.id" :value="d.id">{{ d.name }}</option>
                </select>
              </div>

              <div class="mb-3">
                <label for="position" class="form-label">Position</label>
                <input required type="text" class="form-control" id="position"
                       placeholder="Senior pomidor engineer" v-model="employer.position">
              </div>

              <div class="mb-3">
                <label for="email" class="form-label">Email</label>
                <input required type="email" class="form-control" id="email"
                       placeholder="sultanov@example.com" v-model="employer.email">
              </div>

              <div class="mb-3">
                <label for="phone" class="form-label">Phone</label>
                <input required type="text" class="form-control" id="phone"
                       placeholder="+7 999 999-99-99" v-model="employer.phone">
              </div>

              <div class="mb-3">
                <label for="boss" class="form-label">Boss</label>
                <select required class="form-select" id="boss" aria-label="Select boss" v-model="employer.boss_id">
                  <option :value="null">No boss</option>
                  <option v-for="e in employee" :key="e.id" :value="e.id">{{ e.name }} @{{ e.nickname }}</option>
                </select>
              </div>

              <p class="mb-2">Teams</p>
              <div class="card mb-3">
                <div class="card-body">

                  <div v-for="(t, index) in employer.teams" class="input-group mb-3">
                    <input required type="text" class="form-control" placeholder="Role"
                           aria-label="Role" aria-describedby="role" v-model="t.role">
                    <select required class="form-select" id="team" aria-label="Select team" v-model="t.team">
                      <option v-for="tt in teams" :key="tt.id" :value="tt">{{ tt.name }}</option>
                    </select>
                    <div class="btn btn-primary" @click.prevent="deleteTeamEntry(index)">Remove</div>
                  </div>

                  <div class="btn btn-primary" @click.prevent="addEmptyTeam">Add new team</div>
                </div>
              </div>

              <div class="mb-3">
                <label for="boss" class="form-label">Level</label>
                <select required class="form-select" id="boss" aria-label="Select boss" v-model="employer.level">
                  <option value="user" selected>User</option>
                  <option value="hr" selected>HR</option>
                  <option value="admin" selected>Admin</option>
                </select>
              </div>

              <div class="mb-3">
                <label for="photo" class="form-label">Photo</label>
                <input required type="text" class="form-control" id="photo"
                       placeholder="Image URL here" v-model="employer.photo">
              </div>

              <div class="mb-3">
                <label for="password" class="form-label">Password</label>
                <input required type="password" class="form-control" id="password"
                       placeholder="qwerty" v-model="employerPassword.password">
              </div>

              <div class="mb-3">
                <h1>{{ employer.name }}</h1>
                <h1>{{ employerPassword.password }}</h1>
              </div>

              <button class="btn btn-primary" type="submit">Create</button>
            </form>
          </div>
        </div>

      </div>
      <div class="col-md-2"></div>
    </div>
  </main>
</template>

<script lang="ts">
import {defineComponent} from "vue";
import {mapActions, mapGetters} from "vuex";
import ProfileShort from "@/components/ProfileShort.vue";

export default defineComponent({
  components: {ProfileShort},
  methods: {
    ...mapActions(['loadDepartments', 'loadTeams', 'loadEmployee', 'createEmployer', 'addEmptyTeam', 'deleteTeamEntry']),
  },
  computed: {
    ...mapGetters(['employer', 'departments', 'teams', 'employee', 'employerPassword']),
  },
  created() {
    this.loadDepartments()
    this.loadTeams()
    this.loadEmployee()
  },
});
</script>