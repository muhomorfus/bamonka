<template>
  <main class="container">
      <br>

    <div class="row">
      <div class="col-md-2"></div>
      <div class="col-md-8">

        <div class="card mb-3">
          <div class="card-body">
            <h1>Add vacation</h1>

            <form @submit.prevent="createVacation">
              <div class="mb-3">
                <label for="comment" class="form-label">Comment</label>
                <input required type="text" class="form-control" id="comment"
                       placeholder="Chill & drink beer" v-model="vacation.description">
              </div>

              <div class="mb-3">
                <label for="dates" class="form-label">Dates</label>
                <div class="input-group" id="dates">
                  <input type="text" class="form-control" placeholder="From" v-model="vacation.start">
                  <input type="text" class="form-control" placeholder="To" v-model="vacation.end">
                </div>
              </div>

              <button class="btn btn-primary" type="submit">Add</button>
            </form>
          </div>
        </div>

        <div class="card mb-3" v-for="v in vacations">
          <div class="card-body">
            <h5>{{ v.description }}</h5>
            <p class="mb-0"><b>From: </b>{{ new Date(v.start).toISOString().slice(0, 10) }}</p>
            <p class="mb-0"><b>To: </b>{{ new Date(v.end).toISOString().slice(0, 10) }}</p>
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
    ...mapActions(['loadVacations', 'createVacation', 'getMe']),
  },
  computed: {
    ...mapGetters(['vacations', 'vacation', 'me']),
  },
  mounted() {
    this.getMe().then(() => {
      this.loadVacations(this.me.id)
    })
  },
});
</script>